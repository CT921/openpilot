#!/usr/bin/env python3

import subprocess
import sys
import time

from openpilot.common.params import Params


def trigger_panda_flash():
  """
  Trigger pandad to reflash pandas by:
  1. Clearing PandaSignatures to force signature mismatch
  2. Sending SIGHUP to pandad to restart its connection loop
  """
  params = Params()

  print("Triggering Panda reflash via pandad...")
  print("")

  # Step 1: Clear PandaSignatures to force reflash
  print("[Step 1/2] Clearing PandaSignatures...")
  params.remove("PandaSignatures")
  time.sleep(1)
  print("✓ PandaSignatures cleared")
  print("")

  # Step 2: Find pandad process
  print("[Step 2/2] Signaling pandad to reconnect...")
  result = subprocess.run(
    ["pgrep", "-f", "^./pandad"],
    capture_output=True,
    text=True,
    cwd="/data/openpilot/selfdrive/pandad"
  )

  if result.returncode == 0 and result.stdout.strip():
    pandad_pid = result.stdout.strip().split('\n')[0]
    try:
      # Send SIGINT to gracefully stop pandad
      # Manager will restart it, and it will reflash due to missing signatures
      print(f"Sending SIGINT to pandad (PID: {pandad_pid})...")
      subprocess.run(["kill", "-INT", pandad_pid], check=True)
      print("✓ Signal sent")
      print("")
      print("Waiting for pandad to restart and reflash Panda...")
      print("This may take 30-60 seconds...")

      # Wait for pandad to restart and complete flash
      time.sleep(5)

      # Check if PandaSignatures was restored (meaning flash completed)
      max_wait = 60
      while max_wait > 0:
        sigs = params.get("PandaSignatures")
        if sigs:
          print("")
          print("✓ Panda reflashed successfully!")
          return True
        time.sleep(1)
        max_wait -= 1

        # Show progress
        if max_wait % 10 == 0:
          print(f"  Still waiting... ({60 - max_wait}s elapsed)")

      print("")
      print("⚠ Timeout waiting for flash completion")
      print("Panda may still be flashing, please wait...")
      return False

    except Exception as e:
      print(f"Error signaling pandad: {e}")
      return False
  else:
    print("⚠ pandad not found")
    print("Manager will start pandad and it will reflash automatically")
    return True


if __name__ == "__main__":
  try:
    print("=" * 50)
    print("Panda Flash via pandad")
    print("=" * 50)
    print("")

    success = trigger_panda_flash()

    print("")
    print("=" * 50)
    if success:
      print("Flash trigger completed successfully")
    else:
      print("Flash trigger completed (please check system status)")
    print("=" * 50)

    sys.exit(0 if success else 1)

  except Exception as e:
    print(f"Fatal error: {e}")
    sys.exit(1)
