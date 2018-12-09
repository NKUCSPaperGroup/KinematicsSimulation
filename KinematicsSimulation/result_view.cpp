/**************************************************************
 *  Copyright: Copyright (c) 2018
 *  Created on 2018-12
 *  Author: NKUCSPaperGroup
 *  At: https://github.com/NKUCSPaperGroup
 *  Email: hamiguazzz@qq.com
 **************************************************************/
#include "result_view.h"

QLineSeries* QResultWarp::create_series(const QString& name, const std::vector<double>& x, const std::vector<double>& y)
{
	auto series = new QLineSeries();
	series->setName(name);
	for (int i = 0; i < x.size(); ++i)
	{
		series->append(x[i], y[i]);
	}
	return series;
}

QLineSeries* QResultWarp::create_series(const QString& name, std::pair<std::vector<double>, std::vector<double>>& xy)
{
	return create_series(name, xy.first, xy.second);
}

QtCharts::QChart* QResultWarp::create_chart(const QString& name)
{
	auto re = new QChart;
	re->setTitle(name);
	return re;
}

QValueAxis* QResultWarp::create_axis(const double from, const double to, const QString& name)
{
	auto axis = new QValueAxis;
	axis->setTitleText(name);
	axis->setRange(from, to);
	return axis;
}


auto my_max(const std::vector<double>::const_iterator& begin, const std::vector<double>::const_iterator& end)
{
	auto r = begin;
	auto re = begin;
	for (; r != end; ++r)
	{
		if (*r > *re)re = r;
	}
	return re;
}
auto my_min(const std::vector<double>::const_iterator& begin, const std::vector<double>::const_iterator& end)
{
	auto r = begin;
	auto re = begin;
	for (; r != end; ++r)
	{
		if (*r < *re)re = r;
	}
	return re;
}

QValueAxis* QResultWarp::create_axis(const QString& name, const std::vector<double>& v)
{
	const auto p1 = my_min(v.begin(), v.end());
	const auto p2 =my_max(v.begin(), v.end());
	return create_axis(*p1, *p2, name);
}

QChartView* QResultWarp::create_view(QWidget* parent, QChart* chart, const std::pair<QValueAxis*, QValueAxis*>& axis,
                                     const std::vector<QLineSeries*>& series)
{
	for (auto& line_series : series)
	{
		chart->addSeries(line_series);
		chart->setAxisX(axis.first, line_series);
		chart->setAxisY(axis.second, line_series);
	}
	const auto view = new QChartView{parent};
	view->setChart(chart);
	return view;
}
