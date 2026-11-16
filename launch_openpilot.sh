#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

# On any failure, run the fallback launcher
trap 'exec ./launch_chffrplus.sh' ERR
MODEL="$(tr -d '\0' < "/sys/firmware/devicetree/base/model")"
export MODEL
if [ "$MODEL" = "comma tici" ]; then
  echo "Please use the C3 dedicated branch"
  exit 0
fi
exec ./launch_chffrplus.sh
