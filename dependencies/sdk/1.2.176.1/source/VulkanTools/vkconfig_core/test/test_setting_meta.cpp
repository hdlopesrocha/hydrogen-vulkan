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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "../setting_meta.h"

#include <gtest/gtest.h>

#include <memory>

TEST(test_setting_meta, equal_int) {
    SettingMetaInt data0("data");
    data0.default_value = 76;
    SettingMetaInt data1("data");
    data1.default_value = 76;

    EXPECT_EQ(data0, data1);

    SettingMeta* ptr0 = &data0;
    SettingMeta* ptr1 = &data1;

    EXPECT_EQ(*ptr0, *ptr1);
    EXPECT_EQ(*ptr0, data1);
    EXPECT_EQ(data0, *ptr1);

    SettingMetaInt dataX("dataX");
    dataX.default_value = 76;

    EXPECT_NE(data0, dataX);

    SettingMeta* ptrX = &dataX;
    EXPECT_NE(*ptr0, *ptrX);
    EXPECT_NE(*ptr0, dataX);
    EXPECT_NE(data0, *ptrX);

    SettingMetaInt dataY("data");
    dataY.default_value = 82;

    EXPECT_NE(data0, dataY);

    SettingMeta* ptrY = &dataY;
    EXPECT_NE(*ptr0, *ptrY);
    EXPECT_NE(*ptr0, dataY);
    EXPECT_NE(data0, *ptrY);
}

TEST(test_setting_meta, equal_int_range) {
    SettingMetaFrames data0("data");
    data0.default_value = "6-7";
    SettingMetaFrames data1("data");
    data1.default_value = "6-7";

    EXPECT_EQ(data0, data1);

    SettingMeta* ptr0 = &data0;
    SettingMeta* ptr1 = &data1;

    EXPECT_EQ(*ptr0, *ptr1);
    EXPECT_EQ(*ptr0, data1);
    EXPECT_EQ(data0, *ptr1);

    SettingMetaFrames dataX("dataX");
    dataX.default_value = "6-7";

    EXPECT_NE(data0, dataX);

    SettingMeta* ptrX = &dataX;
    EXPECT_NE(*ptr0, *ptrX);
    EXPECT_NE(*ptr0, dataX);
    EXPECT_NE(data0, *ptrX);

    SettingMetaFrames dataY("data");
    dataY.default_value = "5-7";

    EXPECT_NE(data0, dataY);

    SettingMeta* ptrY = &dataY;
    EXPECT_NE(*ptr0, *ptrY);
    EXPECT_NE(*ptr0, dataY);
    EXPECT_NE(data0, *ptrY);
}

TEST(test_setting_meta, equal_bool) {
    SettingMetaBool data0("data");
    data0.default_value = true;
    SettingMetaBool data1("data");
    data1.default_value = true;

    EXPECT_EQ(data0, data1);

    SettingMeta* ptr0 = &data0;
    SettingMeta* ptr1 = &data1;

    EXPECT_EQ(*ptr0, *ptr1);
    EXPECT_EQ(*ptr0, data1);
    EXPECT_EQ(data0, *ptr1);

    SettingMetaBool dataX("dataX");
    dataX.default_value = true;

    EXPECT_NE(data0, dataX);

    SettingMeta* ptrX = &dataX;
    EXPECT_NE(*ptr0, *ptrX);
    EXPECT_NE(*ptr0, dataX);
    EXPECT_NE(data0, *ptrX);

    SettingMetaBool dataY("data");
    dataY.default_value = false;

    EXPECT_NE(data0, dataY);

    SettingMeta* ptrY = &dataY;
    EXPECT_NE(*ptr0, *ptrY);
    EXPECT_NE(*ptr0, dataY);
    EXPECT_NE(data0, *ptrY);
}

TEST(test_setting_meta, equal_enum) {
    SettingMetaEnum data0("data");
    data0.default_value = "value";
    SettingMetaEnum data1("data");
    data1.default_value = "value";

    EXPECT_EQ(data0, data1);

    SettingMeta* ptr0 = &data0;
    SettingMeta* ptr1 = &data1;

    EXPECT_EQ(*ptr0, *ptr1);
    EXPECT_EQ(*ptr0, data1);
    EXPECT_EQ(data0, *ptr1);

    SettingMetaEnum dataX("dataX");
    dataX.default_value = "value";

    EXPECT_NE(data0, dataX);

    SettingMeta* ptrX = &dataX;
    EXPECT_NE(*ptr0, *ptrX);
    EXPECT_NE(*ptr0, dataX);
    EXPECT_NE(data0, *ptrX);

    SettingMetaEnum dataY("data");
    dataY.default_value = "valueY";

    EXPECT_NE(data0, dataY);

    SettingMeta* ptrY = &dataY;
    EXPECT_NE(*ptr0, *ptrY);
    EXPECT_NE(*ptr0, dataY);
    EXPECT_NE(data0, *ptrY);
}

TEST(test_setting_meta, equal_flags) {
    std::vector<std::string> value;
    value.push_back("valueA");
    value.push_back("valueB");

    std::vector<std::string> valueX;
    valueX.push_back("valueB");
    valueX.push_back("valueA");

    SettingMetaFlags data0("data");
    data0.default_value = value;
    SettingMetaFlags data1("data");
    data1.default_value = value;

    EXPECT_EQ(data0, data1);

    SettingMeta* ptr0 = &data0;
    SettingMeta* ptr1 = &data1;

    EXPECT_EQ(*ptr0, *ptr1);
    EXPECT_EQ(*ptr0, data1);
    EXPECT_EQ(data0, *ptr1);

    SettingMetaFlags dataX("dataX");
    dataX.default_value = value;

    EXPECT_NE(data0, dataX);

    SettingMeta* ptrX = &dataX;
    EXPECT_NE(*ptr0, *ptrX);
    EXPECT_NE(*ptr0, dataX);
    EXPECT_NE(data0, *ptrX);

    std::vector<std::string> valueY;
    value.push_back("valueB");
    value.push_back("valueC");

    SettingMetaFlags dataY("data");
    dataY.default_value = valueY;

    EXPECT_NE(data0, dataY);

    SettingMeta* ptrY = &dataY;
    EXPECT_NE(*ptr0, *ptrY);
    EXPECT_NE(*ptr0, dataY);
    EXPECT_NE(data0, *ptrY);
}

TEST(test_setting_meta, children) {
    SettingMetaSet settings;

    SettingMetaBool& meta_parent0 = settings.Create<SettingMetaBool>("parent0", SETTING_BOOL);
    SettingMetaBool& meta_parent1 = settings.Create<SettingMetaBool>("parent1", SETTING_BOOL);
    SettingMetaInt& meta_child1 = meta_parent1.children.Create<SettingMetaInt>("child1", SETTING_INT);
    meta_child1.default_value = 76;

    EXPECT_TRUE(FindSettingMeta(settings, "child1") != nullptr);
    EXPECT_EQ(SETTING_INT, FindSettingMeta(settings, "child1")->type);

    const SettingMetaInt& meta_child1_found = static_cast<const SettingMetaInt&>(*FindSettingMeta(settings, "child1"));
    EXPECT_EQ(76, meta_child1_found.default_value);
}

TEST(test_setting_meta, is_supported_enum_value_default) {
    SettingEnumValue value;

    EXPECT_EQ(true, IsSupported(&value));
}

TEST(test_setting_meta, is_supported_enum_value_null) {
    const SettingEnumValue* value = nullptr;

    EXPECT_EQ(false, IsSupported(value));
}

TEST(test_setting_meta, is_supported_enum_value_hidden) {
    SettingEnumValue value;
    value.view = SETTING_VIEW_HIDDEN;

    EXPECT_EQ(false, IsSupported(&value));
}

TEST(test_setting_meta, is_supported_enum_value_platform) {
    SettingEnumValue value;
    value.platform_flags = 0;

    EXPECT_EQ(false, IsSupported(&value));
}

TEST(test_setting_meta, is_supported_meta_default) {
    SettingMeta value("key", SETTING_STRING);

    EXPECT_EQ(true, IsSupported(&value));
}

TEST(test_setting_meta, is_supported_meta_null) {
    const SettingMeta* value = nullptr;

    EXPECT_EQ(false, IsSupported(value));
}

TEST(test_setting_meta, is_supported_value_hidden) {
    SettingMeta value("key", SETTING_STRING);
    value.view = SETTING_VIEW_HIDDEN;

    EXPECT_EQ(false, IsSupported(&value));
}

TEST(test_setting_meta, is_supported_value_platform) {
    SettingMeta value("key", SETTING_STRING);
    value.platform_flags = 0;

    EXPECT_EQ(false, IsSupported(&value));
}

TEST(test_setting_meta, process_input_float_no_error) {
    SettingMetaFloat meta("key");
    SettingDataFloat data("key");

    EXPECT_EQ(SETTING_INPUT_NO_ERROR, ProcessInput("1.0", meta, data));
}

TEST(test_setting_meta, process_input_float_error_empty) {
    SettingMetaFloat meta("key");
    SettingDataFloat data("key");

    EXPECT_EQ(SETTING_INPUT_ERROR_EMPTY, ProcessInput("", meta, data));
}

TEST(test_setting_meta, process_input_float_error_syntax) {
    SettingMetaFloat meta("key");
    SettingDataFloat data("key");

    EXPECT_EQ(SETTING_INPUT_ERROR_SYNTAX, ProcessInput("1,0", meta, data));
}

TEST(test_setting_meta, process_input_float_error_sementics) {
    SettingMetaFloat meta("key");
    meta.max_value = 2.0;
    meta.min_value = 1.0;

    SettingDataFloat data("key");

    EXPECT_EQ(SETTING_INPUT_ERROR_SEMENTICS, ProcessInput("4.0", meta, data));
}

TEST(test_setting_meta, process_input_frames_no_error) {
    SettingMetaFrames meta("key");
    SettingDataFrames data("key");

    EXPECT_EQ(SETTING_INPUT_NO_ERROR, ProcessInput("1-2,9", meta, data));
}

TEST(test_setting_meta, process_input_frames_error_empty) {
    SettingMetaFrames meta("key");
    SettingDataFrames data("key");

    EXPECT_EQ(SETTING_INPUT_ERROR_EMPTY, ProcessInput("", meta, data));
}

TEST(test_setting_meta, process_input_frames_error_syntax) {
    SettingMetaFrames meta("key");
    SettingDataFrames data("key");

    EXPECT_EQ(SETTING_INPUT_ERROR_SYNTAX, ProcessInput("a", meta, data));
}
