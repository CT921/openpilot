#!/usr/bin/env bash

export OMP_NUM_THREADS=1
export MKL_NUM_THREADS=1
export NUMEXPR_NUM_THREADS=1
export OPENBLAS_NUM_THREADS=1
export VECLIB_MAXIMUM_THREADS=1
if [ -s /data/params/d/CarModel ] && [ "$(cat /data/params/d/CarModel)" != "[-Not selected-]" ]; then
  export FINGERPRINT="$(cat /data/params/d/CarModel)"
fi

if [ -z "$AGNOS_VERSION" ]; then
  export AGNOS_VERSION="12.8"
fi

export STAGING_ROOT="/data/safe_staging"
