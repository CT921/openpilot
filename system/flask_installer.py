#!/usr/bin/env python3
import subprocess
import importlib.util

def main():
  """Simple Flask installer - no retries, no complex logic"""
  print("Flask installer started")

  # Quick check if Flask already available
  if importlib.util.find_spec("flask") is not None:
    print("Flask already available")
    return

  # Try with dist-packages path
  import sys
  dist_packages_path = '/usr/lib/python3/dist-packages'
  if dist_packages_path not in sys.path:
    sys.path.insert(0, dist_packages_path)
    if importlib.util.find_spec("flask") is not None:
      print("Flask found in dist-packages")
      return

  # Install Flask via pip
  try:
    print("Installing Flask...")
    subprocess.run(['pip', 'install', 'flask'], check=True)
    print("Flask installation completed")
  except subprocess.CalledProcessError as e:
    print(f"Flask installation failed: {e}")
  except Exception as e:
    print(f"Unexpected error: {e}")

if __name__ == "__main__":
  main()
