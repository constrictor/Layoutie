#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtGui>

class Settings
{
public:
	static Settings& instance();

	QColor layerColor(int inLayer) const;
	QColor backgroundColor() const;
	unsigned layerFromOrderNumber(int inOrderNum) const;
private:
	static Settings* mInstance;
};

#endif //SETTINGS_H