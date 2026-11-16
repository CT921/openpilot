# otisserv - Copyright (c) 2019-, Rick Lan, dragonpilot community, and a number of other of contributors.
# Fleet Manager - [actuallylemoncurd](https://github.com/actuallylemoncurd), [AlexandreSato](https://github.com/alexandreSato), [ntegan1](https://github.com/ntegan1), [royjr](https://github.com/royjr), and [sunnyhaibin] (https://github.com/sunnypilot)
# Almost everything else - ChatGPT
# dirty PR pusher - mike8643
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
import os
import subprocess
from pathlib import Path
from openpilot.system.hardware import PC
from openpilot.system.hardware.hw import Paths
from openpilot.system.loggerd.uploader import listdir_by_creation
from openpilot.tools.lib.route import SegmentName

# otisserv conversion
from openpilot.common.params import Params

pi = 3.1415926535897932384626
x_pi = 3.14159265358979324 * 3000.0 / 180.0
a = 6378245.0
ee = 0.00669342162296594323

params = Params()
# path to T.O.P screen recordings and error logs
if PC:
  ERROR_LOGS_PATH = os.path.join(str(Path.home()), ".comma", "community", "crashes", "")
else:
  ERROR_LOGS_PATH = "/data/community/crashes/"


def list_files(path): # still used for footage
  return sorted(listdir_by_creation(path), reverse=True)


def list_file(path): # new function for screenrecords/error-logs
  if os.path.exists(path):
    files = os.listdir(path)
    sorted_files = sorted(files, reverse=True)
  else:
    return []  # Return an empty list if there are no files or directory
  return sorted_files


def is_valid_segment(segment):
  try:
    segment_to_segment_name(Paths.log_root(), segment)
    return True
  except AssertionError:
    return False


def segment_to_segment_name(data_dir, segment):
  fake_dongle = "ffffffffffffffff"
  return SegmentName(str(os.path.join(data_dir, fake_dongle + "|" + segment)))


def all_segment_names():
  segments = []
  for segment in listdir_by_creation(Paths.log_root()):
    try:
      segments.append(segment_to_segment_name(Paths.log_root(), segment))
    except AssertionError:
      pass
  return segments


def all_routes():
  segment_names = all_segment_names()
  route_names = [segment_name.route_name for segment_name in segment_names]
  route_times = [route_name.time_str for route_name in route_names]
  unique_routes = list(dict.fromkeys(route_times))
  return sorted(unique_routes, reverse=True)

def video_to_gif(input_path, output_path, fps=1, duration=6): # not used right now but can if want longer animated gif
  if os.path.exists(output_path):
    return
  command = [
    'ffmpeg', '-y', '-i', input_path,
    '-filter_complex',
    f'fps={fps},scale=240:-1:flags=lanczos,setpts=0.1*PTS,split[s0][s1];[s0]palettegen=max_colors=32[p];[s1][p]paletteuse=dither=bayer',
    '-t', str(duration), output_path
  ]
  subprocess.run(command)
  print(f"GIF file created: {output_path}")

def video_to_img(input_path, output_path, fps=1, duration=6):
  if os.path.exists(output_path):
    return
  subprocess.run(['ffmpeg', '-y', '-i', input_path, '-ss', '5', '-vframes', '1', output_path])
  print(f"GIF file created: {output_path}")

def segments_in_route(route):
  segment_names = [segment_name for segment_name in all_segment_names() if segment_name.time_str == route]
  segments = [segment_name.time_str + "--" + str(segment_name.segment_num) for segment_name in segment_names]
  return segments


def ffmpeg_mp4_concat_wrap_process_builder(file_list, cameratype, chunk_size=1024*512):
  command_line = ["ffmpeg"]
  if not cameratype == "qcamera":
    command_line += ["-f", "hevc"]
  command_line += ["-r", "20"]
  command_line += ["-i", "concat:" + file_list]
  command_line += ["-c", "copy"]
  command_line += ["-map", "0"]
  if not cameratype == "qcamera":
    command_line += ["-vtag", "hvc1"]
  command_line += ["-f", "mp4"]
  command_line += ["-movflags", "empty_moov"]
  command_line += ["-"]
  return subprocess.Popen(
    command_line, stdout=subprocess.PIPE,
    bufsize=chunk_size
  )


def ffmpeg_mp4_wrap_process_builder(filename):
  """Returns a process that will wrap the given filename
     inside a mp4 container, for easier playback by browsers
     and other devices. Primary use case is streaming segment videos
     to the vidserver tool.
     filename is expected to be a pathname to one of the following
       /path/to/a/qcamera.ts
       /path/to/a/dcamera.hevc
       /path/to/a/ecamera.hevc
       /path/to/a/fcamera.hevc
  """
  basename = filename.rsplit("/")[-1]
  extension = basename.rsplit(".")[-1]
  command_line = ["ffmpeg"]
  if extension == "hevc":
    command_line += ["-f", "hevc"]
  command_line += ["-r", "20"]
  command_line += ["-i", filename]
  command_line += ["-c", "copy"]
  command_line += ["-map", "0"]
  if extension == "hevc":
    command_line += ["-vtag", "hvc1"]
  command_line += ["-f", "mp4"]
  command_line += ["-movflags", "empty_moov"]
  command_line += ["-"]
  return subprocess.Popen(
    command_line, stdout=subprocess.PIPE
  )


def ffplay_mp4_wrap_process_builder(file_name):
  command_line = ["ffmpeg"]
  command_line += ["-i", file_name]
  command_line += ["-c", "copy"]
  command_line += ["-map", "0"]
  command_line += ["-f", "mp4"]
  command_line += ["-movflags", "empty_moov"]
  command_line += ["-"]
  return subprocess.Popen(
    command_line, stdout=subprocess.PIPE
  )
