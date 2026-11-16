import os
import operator
import subprocess
import importlib.util
import platform

from cereal import car
from openpilot.common.params import Params
from openpilot.system.hardware import PC, TICI
from openpilot.system.manager.process import PythonProcess, NativeProcess, DaemonProcess
from openpilot.system.hardware.hw import Paths
from openpilot.top.mapd.mapd_manager import MAPD_PATH

WEBCAM = os.getenv("USE_WEBCAM") is not None

def driverview(started: bool, params: Params, CP: car.CarParams) -> bool:
  return started or params.get_bool("IsDriverViewEnabled")

def notcar(started: bool, params: Params, CP: car.CarParams) -> bool:
  return started and CP.notCar

def iscar(started: bool, params: Params, CP: car.CarParams) -> bool:
  return started and not CP.notCar

def logging(started: bool, params: Params, CP: car.CarParams) -> bool:
  run = (not CP.notCar) or not params.get_bool("DisableLogging")
  return started and run

def ublox_available() -> bool:
  return os.path.exists('/dev/ttyHS0') and not os.path.exists('/persist/comma/use-quectel-gps')

def ublox(started: bool, params: Params, CP: car.CarParams) -> bool:
  use_ublox = ublox_available()
  if use_ublox != params.get_bool("UbloxAvailable"):
    params.put_bool("UbloxAvailable", use_ublox)
  return started and use_ublox

def joystick(started: bool, params: Params, CP: car.CarParams) -> bool:
  return started and params.get_bool("JoystickDebugMode")

def not_joystick(started: bool, params: Params, CP: car.CarParams) -> bool:
  return started and not params.get_bool("JoystickDebugMode")

def long_maneuver(started: bool, params: Params, CP: car.CarParams) -> bool:
  return started and params.get_bool("LongitudinalManeuverMode")

def not_long_maneuver(started: bool, params: Params, CP: car.CarParams) -> bool:
  return started and not params.get_bool("LongitudinalManeuverMode")

def qcomgps(started: bool, params: Params, CP: car.CarParams) -> bool:
  return started and not ublox_available()

def always_run(started: bool, params: Params, CP: car.CarParams) -> bool:
  return True

def only_onroad(started: bool, params: Params, CP: car.CarParams) -> bool:
  return started

def only_offroad(started: bool, params: Params, CP: car.CarParams) -> bool:
  return not started

def check_packages_and_install() -> bool:
  # Quick check if Flask & kaitaistruct are installed
  flask_available = importlib.util.find_spec("flask") is not None
  kaitaistruct_available = importlib.util.find_spec("kaitaistruct") is not None

  params = Params()

  if flask_available and kaitaistruct_available:
    if params.get_bool("PackagesInstallRequested"):
      params.put_bool("PackagesInstallRequested", False)

    current_second_boot = params.get_bool("SecondBoot")
    if not current_second_boot:
      params.put_bool("SecondBoot", True)
    return True

  # Start installation only once
  install_requested = params.get_bool("PackagesInstallRequested")
  if not install_requested:
    params.put_bool("PackagesInstallRequested", True)

    current_second_boot = params.get_bool("SecondBoot")
    if current_second_boot:
      params.put_bool("SecondBoot", False)

    packages_to_install = []
    if not flask_available:
      packages_to_install.append("flask")
    if not kaitaistruct_available:
      packages_to_install.append("kaitaistruct")

    try:
      for package in packages_to_install:
        print(f"Installing {package}...")
        result = subprocess.call([
          'python3', '-m', 'pip', 'install', package
        ])

        if result == 0:
          print(f"Successfully installed {package}")
        else:
          print(f"Failed to install {package}")
          params.put_bool("PackagesInstallRequested", False)
          return False

      print(f"All packages installed successfully: {', '.join(packages_to_install)}")

      flask_check = importlib.util.find_spec("flask") is not None
      kaitai_check = importlib.util.find_spec("kaitaistruct") is not None

      if flask_check and kaitai_check:
        params.put_bool("SecondBoot", True)
        return True
      else:
        print("Packages installed but not detected, may need system restart")
        return False

    except Exception as e:
      print(f"Failed to install packages: {e}")
      params.put_bool("PackagesInstallRequested", False)
      return False

  return False

def reset_install_status(params: Params) -> None:
  params.put_bool("PackagesInstallRequested", False)

def flask_ready_and_second_boot(started: bool, params: Params, CP: car.CarParams) -> bool:
  return check_packages_and_install()

def flask_simple_check(started: bool, params: Params, CP: car.CarParams) -> bool:
  return importlib.util.find_spec("flask") is not None

def kaitaistruct_check(started: bool, params: Params, CP: car.CarParams) -> bool:
  return importlib.util.find_spec("kaitaistruct") is not None

def mapd_ready(started: bool, params: Params, CP: car.CarParams) -> bool:
  return bool(os.path.exists(Paths.mapd_root()))

def or_(*fns):
  return lambda *args: operator.or_(*(fn(*args) for fn in fns))

def and_(*fns):
  return lambda *args: operator.and_(*(fn(*args) for fn in fns))

procs = [
  DaemonProcess("manage_athenad", "system.athena.manage_athenad", "AthenadPid"),

  NativeProcess("loggerd", "system/loggerd", ["./loggerd"], logging),
  NativeProcess("encoderd", "system/loggerd", ["./encoderd"], only_onroad),
  NativeProcess("stream_encoderd", "system/loggerd", ["./encoderd", "--stream"], notcar),
  PythonProcess("logmessaged", "system.logmessaged", always_run),

  NativeProcess("camerad", "system/camerad", ["./camerad"], driverview, enabled=not WEBCAM),
  PythonProcess("webcamerad", "tools.webcam.camerad", driverview, enabled=WEBCAM),
  PythonProcess("proclogd", "system.proclogd", only_onroad, enabled=platform.system() != "Darwin"),
  PythonProcess("journald", "system.journald", only_onroad, platform.system() != "Darwin"),
  PythonProcess("micd", "system.micd", iscar, enabled=not os.getenv("LITE")),
  PythonProcess("timed", "system.timed", always_run, enabled=not PC),

  PythonProcess("modeld", "selfdrive.modeld.modeld", only_onroad),
  PythonProcess("dmonitoringmodeld", "selfdrive.modeld.dmonitoringmodeld", driverview, enabled=(WEBCAM or not PC) and not os.getenv("LITE")),

  PythonProcess("sensord", "system.sensord.sensord", only_onroad, enabled=not PC),
  NativeProcess("ui", "selfdrive/ui", ["./ui"], always_run, watchdog_max_dt=(5 if not PC else None)),
  PythonProcess("raylib_ui", "selfdrive.ui.ui", always_run, enabled=False, watchdog_max_dt=(5 if not PC else None)),
  PythonProcess("soundd", "selfdrive.ui.soundd", only_onroad, enabled=not os.getenv("LITE")),
  PythonProcess("locationd", "selfdrive.locationd.locationd", only_onroad),
  NativeProcess("_pandad", "selfdrive/pandad", ["./pandad"], always_run, enabled=False),
  PythonProcess("calibrationd", "selfdrive.locationd.calibrationd", only_onroad),
  PythonProcess("torqued", "selfdrive.locationd.torqued", only_onroad),
  PythonProcess("controlsd", "selfdrive.controls.controlsd", and_(not_joystick, iscar)),
  PythonProcess("joystickd", "tools.joystick.joystickd", or_(joystick, notcar)),
  PythonProcess("selfdrived", "selfdrive.selfdrived.selfdrived", only_onroad),
  PythonProcess("card", "selfdrive.car.card", only_onroad),
  PythonProcess("deleter", "system.loggerd.deleter", always_run),
  PythonProcess("dmonitoringd", "selfdrive.monitoring.dmonitoringd", driverview, enabled=(WEBCAM or not PC) and not os.getenv("LITE")),
  PythonProcess("qcomgpsd", "system.qcomgpsd.qcomgpsd", qcomgps, enabled=TICI),
  PythonProcess("pandad", "selfdrive.pandad.pandad", always_run),
  PythonProcess("paramsd", "selfdrive.locationd.paramsd", only_onroad),
  PythonProcess("lagd", "selfdrive.locationd.lagd", only_onroad),
  PythonProcess("ubloxd", "system.ubloxd.ubloxd", ublox, enabled=TICI),
  PythonProcess("pigeond", "system.ubloxd.pigeond", ublox, enabled=TICI),
  PythonProcess("plannerd", "selfdrive.controls.plannerd", not_long_maneuver),
  PythonProcess("maneuversd", "tools.longitudinal_maneuvers.maneuversd", long_maneuver),
  PythonProcess("radard", "selfdrive.controls.radard", only_onroad),
  PythonProcess("hardwared", "system.hardware.hardwared", always_run),
  PythonProcess("tombstoned", "system.tombstoned", always_run, enabled=not PC),
  PythonProcess("updated", "system.updated.updated", only_offroad, enabled=not PC),
  PythonProcess("uploader", "system.loggerd.uploader", always_run),
  PythonProcess("statsd", "system.statsd", always_run),
  PythonProcess("feedbackd", "selfdrive.ui.feedback.feedbackd", only_onroad, enabled=not os.getenv("LITE")),

  # TOP
  NativeProcess("fleetmanager", "system/fleetmanager", ["./fleet_manager.py"], flask_ready_and_second_boot),
  NativeProcess("mapd", Paths.mapd_root(), ["bash", "-c", f"{MAPD_PATH} > /dev/null 2>&1"], mapd_ready),
  PythonProcess("mapd_manager", "top.mapd.mapd_manager", always_run),
  NativeProcess("locationd_llk", "top/selfdrive/locationd", ["./locationd"], only_onroad),

  # debug procs
  NativeProcess("bridge", "cereal/messaging", ["./bridge"], notcar),
  PythonProcess("webrtcd", "system.webrtc.webrtcd", notcar),
  PythonProcess("webjoystick", "tools.bodyteleop.web", notcar),
  PythonProcess("joystick", "tools.joystick.joystick_control", and_(joystick, iscar)),
]

managed_processes = {p.name: p for p in procs}
