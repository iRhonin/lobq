
# Profile

### Importing things and profile function


```python
import pandas as pd
import matplotlib.pyplot as plt

from profile_ import profile
```

### Profiling for Queue size of 10


```python
result = profile(q_size=10, data_size=1500, levels=5, runs=1000)
result['scaled_time'] = result['time'] / min(result['time'])
result = result.sort_values('scaled_time')
result
```





<table border="1" class="dataframe">
  <thead>
    <tr style="text-align: right;">
      <th></th>
      <th>type_</th>
      <th>time</th>
      <th>time_per_loop</th>
      <th>q_size</th>
      <th>data_size</th>
      <th>levels</th>
      <th>runs</th>
      <th>scaled_time</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <th>0</th>
      <td>C API</td>
      <td>0.000189</td>
      <td>1.257244e-07</td>
      <td>10</td>
      <td>1500</td>
      <td>5</td>
      <td>1000</td>
      <td>1.000000</td>
    </tr>
    <tr>
      <th>2</th>
      <td>Cython</td>
      <td>0.000629</td>
      <td>4.193318e-07</td>
      <td>10</td>
      <td>1500</td>
      <td>5</td>
      <td>1000</td>
      <td>3.335325</td>
    </tr>
    <tr>
      <th>1</th>
      <td>Pure Python</td>
      <td>0.000945</td>
      <td>6.299842e-07</td>
      <td>10</td>
      <td>1500</td>
      <td>5</td>
      <td>1000</td>
      <td>5.010834</td>
    </tr>
    <tr>
      <th>4</th>
      <td>Numpy</td>
      <td>0.000988</td>
      <td>6.587908e-07</td>
      <td>10</td>
      <td>1500</td>
      <td>5</td>
      <td>1000</td>
      <td>5.239959</td>
    </tr>
    <tr>
      <th>3</th>
      <td>Builtin Queue</td>
      <td>0.008946</td>
      <td>5.964163e-06</td>
      <td>10</td>
      <td>1500</td>
      <td>5</td>
      <td>1000</td>
      <td>47.438389</td>
    </tr>
  </tbody>
</table>
</div>




```python
ax = result.plot(kind='barh', x='type_', y='scaled_time', color="slategray", fontsize=13, figsize=(9,5))
ax.set_alpha(0.1)
ax.set_title("Queue Size: 10", fontsize=18);
ax.set_xlabel("CPU Time", fontsize=18);
ax.set_ylabel("Implemention", fontsize=18);
ax.set_xticks([i*5 for i in range(int(max(result['scaled_time']) / 5) + 3)])

for i in ax.patches:
    ax.text(
        i.get_width()+.2, i.get_y()+.31, 
        f'{round((i.get_width()), 2)}x', 
        fontsize=15, color='dimgrey',
    )
    
ax.invert_yaxis()

```


![png](notebook/output_5_0.png)


### Profiling for Queue size of 200


```python
result = profile(q_size=200, data_size=1500, levels=5, runs=100)
result['scaled_time'] = result['time'] / min(result['time'])
result = result.sort_values('scaled_time')
result
```




<div>
<table border="1" class="dataframe">
  <thead>
    <tr style="text-align: right;">
      <th></th>
      <th>type_</th>
      <th>time</th>
      <th>time_per_loop</th>
      <th>q_size</th>
      <th>data_size</th>
      <th>levels</th>
      <th>runs</th>
      <th>scaled_time</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <th>0</th>
      <td>C API</td>
      <td>0.000193</td>
      <td>1.289897e-07</td>
      <td>200</td>
      <td>1500</td>
      <td>5</td>
      <td>100</td>
      <td>1.000000</td>
    </tr>
    <tr>
      <th>2</th>
      <td>Cython</td>
      <td>0.000513</td>
      <td>3.422819e-07</td>
      <td>200</td>
      <td>1500</td>
      <td>5</td>
      <td>100</td>
      <td>2.653559</td>
    </tr>
    <tr>
      <th>1</th>
      <td>Pure Python</td>
      <td>0.000841</td>
      <td>5.607256e-07</td>
      <td>200</td>
      <td>1500</td>
      <td>5</td>
      <td>100</td>
      <td>4.347056</td>
    </tr>
    <tr>
      <th>4</th>
      <td>Numpy</td>
      <td>0.000937</td>
      <td>6.246372e-07</td>
      <td>200</td>
      <td>1500</td>
      <td>5</td>
      <td>100</td>
      <td>4.842534</td>
    </tr>
    <tr>
      <th>3</th>
      <td>Builtin Queue</td>
      <td>0.008379</td>
      <td>5.586129e-06</td>
      <td>200</td>
      <td>1500</td>
      <td>5</td>
      <td>100</td>
      <td>43.306771</td>
    </tr>
  </tbody>
</table>
</div>




```python
ax = result.plot(kind='barh', x='type_', y='scaled_time', color="slategray", fontsize=13, figsize=(9,5))
ax.set_alpha(0.1)
ax.set_title("Queue Size:200", fontsize=18);
ax.set_xlabel("CPU Time", fontsize=18);
ax.set_ylabel("Implemention", fontsize=18);
ax.set_xticks([i*5 for i in range(int(max(result['scaled_time']) / 5) + 3)])

for i in ax.patches:
    ax.text(
        i.get_width()+.2, i.get_y()+.31, 
        f'{round((i.get_width()), 2)}x', 
        fontsize=15, color='dimgrey',
    )
    
ax.invert_yaxis()

```


![png](notebook/output_8_0.png)

