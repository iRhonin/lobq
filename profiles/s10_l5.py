import pandas as pd

from profiles import profile


results = pd.DataFrame()

results = results.append(profile(10, 1500, 5, 100))
results = results.append(profile(200, 1500, 5, 100))
print(results)

