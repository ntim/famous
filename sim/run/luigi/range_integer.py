#!/usr/bin/env python
import luigi
import datetime
import itertools
import time

class RangeInteger(luigi.tools.range.RangeBase):
    start = luigi.Parameter(default=1)
    stop = luigi.Parameter(default=int(2 ** 16))
    task_limit = luigi.IntParameter(int(2 ** 12))
    task_kwargs = luigi.Parameter(default=[])
    
    def _instantiate_task_cls(self, param):
        task_kwargs = {}
        if len(self.task_kwargs) > int(param) - self.start:
            task_kwargs = self.task_kwargs[int(param) - self.start]
        if self.param_name is None:
            return self.of_cls(param, **task_kwargs)
        else:
            kwargs = {self.param_name: param}
            kwargs.update(task_kwargs)
            return self.of_cls(**kwargs)

    def moving_start(self, now):
        return self.parameter_to_datetime(int(self.start))

    def moving_stop(self, now):
        return self.parameter_to_datetime(int(self.stop))

    def datetime_to_parameter(self, dt):
        return time.mktime(dt.timetuple())

    def parameter_to_datetime(self, p):
        return datetime.datetime.fromtimestamp(int(p))

    def finite_datetimes(self, finite_start, finite_stop):
        date_start = datetime.datetime(finite_start.year, finite_start.month, finite_start.day)
        dates = []
        for i in itertools.count():
            t = date_start + datetime.timedelta(seconds=i)
            if t >= finite_stop:
                return dates
            if t >= finite_start:
                dates.append(t)
