/*
 *    This file is part of layoutie.
 *
 *    layoutie is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    layoutie is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with layoutie.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef COMPONENT_ITEM_H
#define COMPONENT_ITEM_H

class QGraphicsItemGroup;

class ComponentItem
{
public:
    ComponentItem();

    virtual ~ComponentItem();

    void update();

    QGraphicsItemGroup*    mainItem();
    QGraphicsItemGroup*    groundPlaneItem();
    QGraphicsItemGroup*    drillItem();
protected:
    virtual void createItem(QGraphicsItemGroup* inOutItem, bool inIsMainNotGround) = 0;
    virtual void createDrillItem(QGraphicsItemGroup* inOutItem); // default implementation is empty.
private:
    void cleanSubItems(QGraphicsItemGroup* inOutItem);

    QGraphicsItemGroup* mMainItem;
    QGraphicsItemGroup* mGroundPlaneItem;
    QGraphicsItemGroup* mDrillItem;
};

#endif //COMPONENT_ITEM_H