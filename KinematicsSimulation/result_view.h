/**************************************************************
 *  Copyright: Copyright (c) 2018
 *  Created on 2018-12
 *  Author: NKUCSPaperGroup
 *  At: https://github.com/NKUCSPaperGroup
 *  Email: hamiguazzz@qq.com
 **************************************************************/
#pragma once
#include <QtWidgets>
#include <QtCharts>
#include <vector>
#include "kinematics_lib.h"

namespace QResultWarp
{
	template <typename E, typename F>
	std::vector<double> map_to_double(const std::vector<E>& origin, const F& f)
	{
		std::vector<double> data;
		for (int i = 0; i < origin.size(); ++i)
		{
			data.push_back(f(origin[i]));
		}
		return move(data);
	}

	template <typename F>
	std::pair<std::vector<double>, std::vector<double>>
		map_to_series(const std::pair<physics_scene::time_sequence, physics_scene::result> origin, const F& f)
	{
		return { origin.first, map_to_double(*origin.second->history(), f) };
	}

	QLineSeries* create_series(const QString& name, const std::vector<double>& x, const std::vector<double>& y);

	QLineSeries* create_series(const QString& name, std::pair<std::vector<double>, std::vector<double>>& xy);

	QtCharts::QChart* create_chart(const QString& name);

	QValueAxis* create_axis(const double from, const double to, const QString& name);

	QValueAxis* create_axis(const QString& name, const std::vector<double>& v);

	QChartView* create_view(QWidget* parent, QChart* chart
		, const std::pair<QValueAxis*, QValueAxis*>& axis
		, const std::vector<QLineSeries*>& series);
};
