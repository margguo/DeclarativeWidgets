/*
  Copyright (C) 2012 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Kevin Krammer, krake@kdab.com
  Author: Tobias Koenig, tokoe@kdab.com

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include "declarativegridlayout_p.h"

class DeclarativeGridLayoutAttached::Private
{
  public:
    Private(QWidget *w, QLayout *l)
      : row(0), column(0), rowSpan(1), columnSpan(1), alignment(0),
        widget(w), layout(l)
    {}

    int row;
    int column;
    int rowSpan;
    int columnSpan;
    Qt::Alignment alignment;

    QPointer<QWidget> widget;
    QPointer<QLayout> layout;
    QPointer<QGridLayout> parentLayout;
};

DeclarativeGridLayoutAttached::DeclarativeGridLayoutAttached(QWidget *widget, QObject *parent)
  : QObject(parent), d(new Private(widget, 0))
{
}

DeclarativeGridLayoutAttached::DeclarativeGridLayoutAttached(QLayout *layout, QObject *parent)
  : QObject(parent), d(new Private(0, layout))
{
}

DeclarativeGridLayoutAttached::~DeclarativeGridLayoutAttached()
{
  delete d;
}

void DeclarativeGridLayoutAttached::setParentLayout(QGridLayout *parentLayout)
{
  d->parentLayout = parentLayout;
}

void DeclarativeGridLayoutAttached::setRow(int row)
{
  if (row == d->row)
    return;

  d->row = row;
  emit rowChanged(row);
}

int DeclarativeGridLayoutAttached::row() const
{
  return d->row;
}

void DeclarativeGridLayoutAttached::setColumn(int column)
{
  if (column == d->column)
    return;

  d->column = column;
  emit columnChanged(column);
}

int DeclarativeGridLayoutAttached::column() const
{
  return d->column;
}

void DeclarativeGridLayoutAttached::setRowSpan(int rowSpan)
{
  if (rowSpan == d->rowSpan)
    return;

  d->rowSpan = rowSpan;
  emit rowSpanChanged(rowSpan);
}

int DeclarativeGridLayoutAttached::rowSpan() const
{
  return d->rowSpan;
}

void DeclarativeGridLayoutAttached::setColumnSpan(int columnSpan)
{
  if (columnSpan == d->columnSpan)
    return;

  d->columnSpan = columnSpan;
  emit columnSpanChanged(columnSpan);
}

int DeclarativeGridLayoutAttached::columnSpan() const
{
  return d->columnSpan;
}

void DeclarativeGridLayoutAttached::setAlignment(Qt::Alignment alignment)
{
  if (alignment == d->alignment)
    return;

  d->alignment = alignment;
  emit alignmentChanged(alignment);

  if (d->parentLayout) {
    if (d->widget)
      d->parentLayout->setAlignment(d->widget, d->alignment);

    if (d->layout)
      d->parentLayout->setAlignment(d->layout, d->alignment);
  }
}

Qt::Alignment DeclarativeGridLayoutAttached::alignment() const
{
  return d->alignment;
}

// DeclarativeGridLayout
DeclarativeGridLayout::DeclarativeGridLayout(QObject *parent) : DeclarativeLayoutProxy<QGridLayout>(parent)
{
  connectAllSignals(m_proxiedObject, this);
}

DeclarativeGridLayoutAttached *DeclarativeGridLayout::qmlAttachedProperties(QObject *parent)
{
  AbstractDeclarativeObject *declarativeObject = dynamic_cast<AbstractDeclarativeObject*>(parent);
  if (declarativeObject) {
    QWidget *widget = qobject_cast<QWidget*>(declarativeObject->object());
    if (widget)
      return new DeclarativeGridLayoutAttached(widget, parent);

    QLayout *layout = qobject_cast<QLayout*>(declarativeObject->object());
    if (layout)
      return new DeclarativeGridLayoutAttached(layout, parent);
  }

  qmlInfo(parent) << "Can only attach GridLayout to widgets and layouts";
  return 0;
}

void DeclarativeGridLayout::addWidget(QWidget *widget, AbstractDeclarativeObject *declarativeObject)
{
  int row = 0;
  int column = 0;
  int rowSpan = 1;
  int columnSpan = 1;
  Qt::Alignment alignment = 0;

  QObject *attachedProperties = qmlAttachedPropertiesObject<DeclarativeGridLayout>(declarativeObject, false);
  DeclarativeGridLayoutAttached *properties = qobject_cast<DeclarativeGridLayoutAttached*>(attachedProperties);
  if (properties) {
    row = properties->row();
    column = properties->column();
    rowSpan = properties->rowSpan();
    columnSpan = properties->columnSpan();
    alignment = properties->alignment();

    properties->setParentLayout(m_proxiedObject);
  }

  m_proxiedObject->addWidget(widget, row, column, rowSpan, columnSpan, alignment);
  m_children.append(declarativeObject);
}

void DeclarativeGridLayout::addLayout(QLayout *layout, AbstractDeclarativeObject *declarativeObject)
{
  int row = 0;
  int column = 0;
  int rowSpan = 1;
  int columnSpan = 1;
  Qt::Alignment alignment = 0;

  QObject *attachedProperties = qmlAttachedPropertiesObject<DeclarativeGridLayout>(declarativeObject, false);
  DeclarativeGridLayoutAttached *properties = qobject_cast<DeclarativeGridLayoutAttached*>(attachedProperties);
  if (properties) {
    row = properties->row();
    column = properties->column();
    rowSpan = properties->rowSpan();
    columnSpan = properties->columnSpan();
    alignment = properties->alignment();

    properties->setParentLayout(m_proxiedObject);
  }

  m_proxiedObject->addLayout(layout, row, column, rowSpan, columnSpan, alignment);
  m_children.append(declarativeObject);
}

CUSTOM_METAOBJECT(DeclarativeGridLayout, QGridLayout)
