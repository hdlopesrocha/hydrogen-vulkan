/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Authors:
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "widget_setting_flags.h"
#include "widget_setting.h"

#include "../vkconfig_core/layer.h"
#include "../vkconfig_core/util.h"

#include <QPushButton>

#include <cassert>

WidgetSettingFlag::WidgetSettingFlag(QTreeWidget* tree, QTreeWidgetItem* item, const SettingMetaFlags& meta,
                                     SettingDataSet& data_set, const std::string& flag)
    : WidgetSettingBase(tree, item),
      meta(meta),
      data(*data_set.Get<SettingDataFlags>(meta.key.c_str())),
      data_set(data_set),
      flag(flag),
      field(new QCheckBox(this)) {
    assert(&data);
    assert(!flag.empty());

    const SettingEnumValue* enum_value = FindByKey(meta.enum_values, flag.c_str());
    assert(enum_value);

    this->field->setText(enum_value->label.c_str());
    this->field->setToolTip(enum_value->description.c_str());
    this->field->setFont(tree->font());
    this->field->show();
    this->connect(this->field, SIGNAL(clicked(bool)), this, SLOT(OnClicked(bool)));

    tree->setItemWidget(this->item, 0, this);
    item->setSizeHint(0, QSize(0, ITEM_HEIGHT));

    this->Refresh(REFRESH_ENABLE_AND_STATE);
}

void WidgetSettingFlag::Refresh(RefreshAreas refresh_areas) {
    const bool enabled = ::CheckDependence(this->meta, data_set);

    this->item->setDisabled(!enabled);
    this->field->setEnabled(enabled);
    this->setEnabled(enabled);

    if (refresh_areas == REFRESH_ENABLE_AND_STATE) {
        this->field->blockSignals(true);
        this->field->setChecked(std::find(data.value.begin(), data.value.end(), flag) != data.value.end());
        this->field->blockSignals(false);
    }
}

void WidgetSettingFlag::OnClicked(bool checked) {
    if (checked) {
        AppendString(data.value, flag);
    } else {
        RemoveString(data.value, flag);
    }

    emit itemChanged();
}
