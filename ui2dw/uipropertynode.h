/*
  Copyright (C) 2013-2014 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Kevin Krammer, kevin.krammer@kdab.com
  Author: Tobias Koenig, tobias.koenig@kdab.com

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

#ifndef UIPROPERTYNODE_H
#define UIPROPERTYNODE_H

#include "uinode.h"

#include <QHash>
#include <QMargins>
#include <QStringList>
#include <QVariant>

class Parser;
class PropertyValueParser;

Q_DECLARE_METATYPE(QMargins);

struct EnumValue
{
    QStringList nameParts;
};

Q_DECLARE_METATYPE(EnumValue)

struct SetValue
{
    QList<EnumValue> flags;
};

Q_DECLARE_METATYPE(SetValue)

struct FontValue
{
    QVariantHash fontProperties;
};

Q_DECLARE_METATYPE(FontValue)

class UiPropertyNode : public UiNode
{
  public:
    UiPropertyNode();

    static UiNode *parse(Parser *parser);

    void accept(UiNodeVisitor *visitor);

    QVariant value() const;
    void setValue(const QVariant &value);

  protected:
    QVariant m_value;

    typedef QHash<QString, PropertyValueParser*> ValueParserHash;
    static ValueParserHash s_valueParsers;

  protected:
    static void initializeValueParsers();
};

#endif // UIPROPERTYNODE_H
