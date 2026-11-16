filenames = ['/data/openpilot/selfdrive/car/top_tmp/AllCars']

with open('/data/openpilot/selfdrive/car/top_tmp/Cars', 'w') as outfile:
  for names in filenames:
    with open(names) as infile:
      outfile.write(infile.read())
    outfile.write("\n")
