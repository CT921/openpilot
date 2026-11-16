import os
import capnp
from opendbc.car.common.basedir import BASEDIR

# TODO: remove car from cereal/__init__.py and always import from opendbc
try:
  from cereal import car
except ImportError:
  capnp.remove_import_hook()
  car = capnp.load(os.path.join(BASEDIR, "car.capnp"))

CarState = car.CarState
RadarData = car.RadarData
CarControl = car.CarControl
CarParams = car.CarParams

CarStateT = capnp.lib.capnp._StructModule
RadarDataT = capnp.lib.capnp._StructModule
CarControlT = capnp.lib.capnp._StructModule
CarParamsT = capnp.lib.capnp._StructModule


class TopFlags:
  LateralALKA = 1
  NNFF = 2
  ToyotaStockLong = 2 ** 2
  ToyotaTSSPTune = 2 ** 3
  ToyotaAutoLock = 2 ** 4
  ToyotaAutoUnlock = 2 ** 5
  ToyotaReverseAccChange = 2 ** 6
  ToyotaTSSPSNG = 2 ** 7
  ToyotaBSM = 2 ** 8
  ToyotaAutoBrakeHold = 2 ** 9
  ToyotaExperimentalMode = 2 ** 10
  ToyotaDriveMode = 2 ** 11
