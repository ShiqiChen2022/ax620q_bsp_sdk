/**************************************************************************************************
 *
 * Copyright (c) 2019-2023 Axera Semiconductor (Shanghai) Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor (Shanghai) Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor (Shanghai) Co., Ltd.
 *
 **************************************************************************************************/

#pragma once

#include <cstdint>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>

namespace preprocess
{
    // opencv mat(h, w)
    // resize cv::Size(dstw, dsth)
    void get_input_data_no_letterbox(const cv::Mat& mat, std::vector<uint8_t>& image, int model_h, int model_w, bool bgr2rgb = false)
    {
        cv::Mat img_new(model_h, model_w, CV_8UC3, image.data());
        cv::resize(mat, img_new, cv::Size(model_w, model_h));
        if (bgr2rgb)
        {
            cv::cvtColor(img_new, img_new, cv::COLOR_BGR2RGB);
        }
    }

    void get_input_data_letterbox(cv::Mat mat, std::vector<uint8_t>& image, int letterbox_rows, int letterbox_cols, bool bgr2rgb = false)
    {
        /* letterbox process to support different letterbox size */
        float scale_letterbox;
        int resize_rows;
        int resize_cols;
        if ((letterbox_rows * 1.0 / mat.rows) < (letterbox_cols * 1.0 / mat.cols))
        {
            scale_letterbox = (float)letterbox_rows * 1.0f / (float)mat.rows;
        }
        else
        {
            scale_letterbox = (float)letterbox_cols * 1.0f / (float)mat.cols;
        }
        resize_cols = int(scale_letterbox * (float)mat.cols);
        resize_rows = int(scale_letterbox * (float)mat.rows);

        cv::Mat img_new(letterbox_rows, letterbox_cols, CV_8UC3, image.data());

        cv::resize(mat, mat, cv::Size(resize_cols, resize_rows));

        int top = (letterbox_rows - resize_rows) / 2;
        int bot = (letterbox_rows - resize_rows + 1) / 2;
        int left = (letterbox_cols - resize_cols) / 2;
        int right = (letterbox_cols - resize_cols + 1) / 2;

        // Letterbox filling
        cv::copyMakeBorder(mat, img_new, top, bot, left, right, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
        if (bgr2rgb)
        {
            cv::cvtColor(img_new, img_new, cv::COLOR_BGR2RGB);
        }
    }

    void get_input_data_centercrop(cv::Mat mat, std::vector<uint8_t>& image, int model_h, int model_w, bool bgr2rgb = false)
    {
        /* letterbox process to support different letterbox size */

        /* C2C BGR */
        if (mat.channels() == 4)
        {
            cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);
        }

        if (mat.channels() == 1)
        {
            cv::cvtColor(mat, mat, cv::COLOR_GRAY2BGR);
        }

        /* Center */
        int h0;
        int w0;
        if (mat.rows < mat.cols)
        {
            h0 = 256;
            w0 = int(mat.cols * (256.0 / mat.rows));
        }
        else
        {
            h0 = int(mat.rows * (256.0 / mat.cols));
            w0 = 256;
        }
        int center_h = int(h0 / 2);
        int center_w = int(w0 / 2);

        cv::resize(mat, mat, cv::Size(w0, h0));

        // cv::imwrite("center.jpg", mat);

        /* Crop */
        cv::Rect crop_box(center_w - int(model_w / 2), center_h - int(model_h / 2), model_w, model_h);
        cv::Mat img_new(model_h, model_w, CV_8UC3, image.data());

        cv::Mat mat_crop = mat(crop_box).clone();

        // cv::imwrite("mat_crop.jpg", mat_crop);
        mat_crop.copyTo(img_new);

        // cv::imwrite("img_new.jpg", img_new);

        /* SwapRB*/
        if (bgr2rgb)
        {
            cv::cvtColor(img_new, img_new, cv::COLOR_BGR2RGB);
        }
    }
} // namespace preprocess