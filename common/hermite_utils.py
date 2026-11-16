"""
Hermite interpolation utilities for smooth acceleration/deceleration curves
"""

import numpy as np
from openpilot.common.swaglog import cloudlog


def compute_symmetric_slopes(x, y):
  """
  Compute symmetric slopes for Hermite interpolation

  Args:
    x: Array of x coordinates (must be monotonically increasing)
    y: Array of y values corresponding to x

  Returns:
    Array of slopes at each point
  """
  n = len(x)
  if n < 2:
    raise ValueError("At least two points are required to compute slopes")

  if len(x) != len(y):
    raise ValueError(f"x and y must have same length, got x:{len(x)}, y:{len(y)}")

  m = np.zeros(n)
  for i in range(n):
    if i == 0:
      # Forward difference for first point
      m[i] = (y[i+1] - y[i]) / (x[i+1] - x[i])
    elif i == n-1:
      # Backward difference for last point
      m[i] = (y[i] - y[i-1]) / (x[i] - x[i-1])
    else:
      # Average of forward and backward differences for interior points
      m[i] = ((y[i+1] - y[i]) / (x[i+1] - x[i]) + (y[i] - y[i-1]) / (x[i] - x[i-1])) / 2
  return m


def hermite_interpolate(x, xp, yp, slopes):
  """
  Perform Hermite cubic interpolation

  Args:
    x: Point(s) at which to evaluate the interpolation
    xp: Array of x coordinates of known points
    yp: Array of y values at known points
    slopes: Array of slopes at known points

  Returns:
    Interpolated value(s) at x
  """
  # Input validation
  if len(xp) != len(yp) or len(xp) != len(slopes):
    raise ValueError("xp, yp and slopes must have same length")

  if len(xp) < 2:
    raise ValueError("At least two points are required for interpolation")

  x = np.clip(x, xp[0], xp[-1])

  idx = np.searchsorted(xp, x) - 1
  idx = np.clip(idx, 0, len(slopes) - 2)

  x0, x1 = xp[idx], xp[idx+1]
  y0, y1 = yp[idx], yp[idx+1]
  m0, m1 = slopes[idx], slopes[idx+1]

  if x1 - x0 == 0:
    return float(y0)

  t = (x - x0) / (x1 - x0)
  h00 = 2*t**3 - 3*t**2 + 1
  h10 = t**3 - 2*t**2 + t
  h01 = -2*t**3 + 3*t**2
  h11 = t**3 - t**2

  interpolated = (h00 * y0) + (h10 * (x1 - x0) * m0) + (h01 * y1) + (h11 * (x1 - x0) * m1)
  return float(interpolated)


def create_hermite_interpolator(xp, yp, name=None):
  """
  Create a Hermite interpolator function with pre-computed slopes

  Args:
    xp: Array of x coordinates of known points
    yp: Array of y values at known points
    name: Optional name for logging purposes

  Returns:
    Tuple of (interpolator_function, success_flag)
    The interpolator function will always return valid values,
    using linear interpolation as fallback if Hermite fails
  """
  name_str = f" for {name}" if name else ""

  try:
    slopes = compute_symmetric_slopes(xp, yp)

    def hermite_interpolator(x):
      try:
        return hermite_interpolate(x, xp, yp, slopes)
      except Exception:
        # Silent fallback to linear interpolation
        return np.interp(x, xp, yp)

    return hermite_interpolator, True

  except Exception as e:
    cloudlog.warning(f"Cannot create Hermite interpolator{name_str}: {e}, using linear interpolation")

    def linear_interpolator(x):
      return np.interp(x, xp, yp)

    return linear_interpolator, False


def create_safe_interpolator(xp, yp, name=None, prefer_hermite=True):
  """
  Create a safe interpolator that never fails

  Args:
    xp: Array of x coordinates of known points
    yp: Array of y values at known points
    name: Optional name for logging purposes
    prefer_hermite: If True, try Hermite first, otherwise use linear

  Returns:
    Interpolator function that always returns valid values
  """
  if prefer_hermite:
    interpolator, _ = create_hermite_interpolator(xp, yp, name)
    return interpolator
  else:
    return lambda x: np.interp(x, xp, yp)
