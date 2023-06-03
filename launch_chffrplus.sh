#!/usr/bin/bash

if [ -z "$BASEDIR" ]; then
  BASEDIR="/data/openpilot"
fi

source "$BASEDIR/launch_env.sh"

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

function agnos_init {
  # TODO: move this to agnos
  sudo rm -f /data/etc/NetworkManager/system-connections/*.nmmeta

  # set success flag for current boot slot
  sudo abctl --set_success

  # Check if AGNOS update is required
  if [ $(< /VERSION) != "$AGNOS_VERSION" ]; then
    AGNOS_PY="$DIR/system/hardware/tici/agnos.py"
    MANIFEST="$DIR/system/hardware/tici/agnos.json"
    if $AGNOS_PY --verify $MANIFEST; then
      sudo reboot
    fi
    $DIR/system/hardware/tici/updater $AGNOS_PY $MANIFEST
  fi

  # install missing libs
  LIB_PATH="/data/openpilot/selfdrive/mapd/assets"
  PY_LIB_DEST="/lib/python3.8/site-packages"
  sudo mount -o rw,remount /
  # mapd
  MODULE="opspline"
  if [ ! -d "$PY_LIB_DEST/$MODULE" ]; then
    echo "Installing $MODULE..."
    tar -zxvf "$LIB_PATH/$MODULE.tar.gz" -C "$PY_LIB_DEST/"
  fi
  MODULE="overpy"
  if [ ! -d "$PY_LIB_DEST/$MODULE" ]; then
    echo "Installing $MODULE..."
    tar -zxvf "$LIB_PATH/$MODULE.tar.gz" -C "$PY_LIB_DEST/"
  fi
  sudo mount -o ro,remount /

  # mapd osm server
  MODULE="osm-3s_v0.7.56"
  if [ ! -d /data/media/0/osm/ ]; then
    sudo mount -o rw,remount /
    sudo tar -vxf "/data/openpilot/selfdrive/mapd/assets/$MODULE.tar.xz" -C /data/media/0/
    sudo mv "/data/media/0/$MODULE" /data/media/0/osm
    sudo mount -o ro,remount /
  fi
}

function launch {
  # Remove orphaned git lock if it exists on boot
  [ -f "$DIR/.git/index.lock" ] && rm -f $DIR/.git/index.lock

  # Pull time from panda
  $DIR/selfdrive/boardd/set_time.py

  # Check to see if there's a valid overlay-based update available. Conditions
  # are as follows:
  #
  # 1. The BASEDIR init file has to exist, with a newer modtime than anything in
  #    the BASEDIR Git repo. This checks for local development work or the user
  #    switching branches/forks, which should not be overwritten.
  # 2. The FINALIZED consistent file has to exist, indicating there's an update
  #    that completed successfully and synced to disk.

  if [ -f "${BASEDIR}/.overlay_init" ]; then
    find ${BASEDIR}/.git -newer ${BASEDIR}/.overlay_init | grep -q '.' 2> /dev/null
    if [ $? -eq 0 ]; then
      echo "${BASEDIR} has been modified, skipping overlay update installation"
    else
      if [ -f "${STAGING_ROOT}/finalized/.overlay_consistent" ]; then
        if [ ! -d /data/safe_staging/old_openpilot ]; then
          echo "Valid overlay update found, installing"
          LAUNCHER_LOCATION="${BASH_SOURCE[0]}"

          mv $BASEDIR /data/safe_staging/old_openpilot
          mv "${STAGING_ROOT}/finalized" $BASEDIR
          cd $BASEDIR

          echo "Restarting launch script ${LAUNCHER_LOCATION}"
          unset AGNOS_VERSION
          exec "${LAUNCHER_LOCATION}"
        else
          echo "openpilot backup found, not updating"
          # TODO: restore backup? This means the updater didn't start after swapping
        fi
      fi
    fi
  fi

  # handle pythonpath
  ln -sfn $(pwd) /data/pythonpath
  export PYTHONPATH="$PWD"

  # hardware specific init
  agnos_init

  # write tmux scrollback to a file
  tmux capture-pane -pq -S-1000 > /tmp/launch_log

  python ./selfdrive/car/honda/values.py > /data/openpilot/selfdrive/car/top_tmp/HondaCars
  python ./selfdrive/car/hyundai/values.py > /data/openpilot/selfdrive/car/top_tmp/HyundaiCars
  python ./selfdrive/car/subaru/values.py > /data/openpilot/selfdrive/car/top_tmp/SubaruCars
  python ./selfdrive/car/toyota/values.py > /data/openpilot/selfdrive/car/top_tmp/ToyotaCars
  python ./selfdrive/car/volkswagen/values.py > /data/openpilot/selfdrive/car/top_tmp/VolkswagenCars

  python ./force_car_recognition.py

  # start manager
  cd selfdrive/manager
  chmod 777 custom_dep.py
  ./custom_dep.py && ./build.py && ./manager.py

  # if broken, keep on screen error
  while true; do sleep 1; done
}

launch
