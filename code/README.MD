# توضیح بخش‌های مختلف کد

## process_data.py

در این فایل پردازش داده‌ه‌ها به صورت زیر انجام می‌شود.

داده‌های زمانی براساس پارامتر مشخصی گسسته‌سازی می‌شوند و به ازای هر 
time_stamp
زمان یک برچسب مشخص می‌شود.

روش تضمیم‌گیری برای برچسب به این‌صورت است که اگر بیش از تعداد مشخصی از یال‌های آن دسته‌ی زمانی برچسب مثبت داشته باشند آن لحظه‌ی زمانی به صورت کامل مثبت اعلام می‌شود.

```python
def generate_final_labels(edge_threshold, record_labels, time_param):
    data = pd.DataFrame(np.array(record_labels))
    labels = []
    data[2] = (data[2] / time_param).astype(int)

    for i in pd.unique(data[2]):
        labels.append(sum(data[data[2] == i][3]))

    labels = np.array(labels)
    labels = labels >= edge_threshold
    labels = labels * 1
    return labels
```