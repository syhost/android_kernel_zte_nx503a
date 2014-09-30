/* Copyright (c) 2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include "msm_sensor.h"
#define IMX214_OIS_SHARP_SENSOR_NAME "imx214_ois_sharp"
DEFINE_MSM_MUTEX(imx214_ois_sharp_mut);

static struct msm_sensor_ctrl_t imx214_ois_sharp_s_ctrl;

static struct msm_sensor_power_setting imx214_ois_sharp_power_setting[] = {
#if defined(CONFIG_ZTE_CAMERA_Z7) || defined(CONFIG_ZTE_CAMERA_Z7MINI) || defined(CONFIG_ZTE_CAMERA_NX507J) \
					|| defined(CONFIG_ZTE_CAMERA_NX505J) || defined(CONFIG_ZTE_CAMERA_NX506J) 
	{
		.seq_type = SENSOR_GPIO,
		.seq_val = SENSOR_GPIO_VDIG,
		.config_val = GPIO_OUT_LOW,
		.delay = 1,
	},
	{
		.seq_type = SENSOR_GPIO,
		.seq_val = SENSOR_GPIO_VDIG,
		.config_val = GPIO_OUT_HIGH,
		.delay = 1,
	},
#endif
#ifdef CONFIG_ZTE_CAMERA_NX506J
#else
	{
		.seq_type = SENSOR_VREG,
		.seq_val = CAM_VDIG,
		.config_val = 0,
		.delay = 0,
	},
#endif
	{
		.seq_type = SENSOR_VREG,
		.seq_val = CAM_VANA,
		.config_val = 0,
		.delay = 0,
	},
	{
		.seq_type = SENSOR_VREG,
		.seq_val = CAM_VIO,
		.config_val = 0,
		.delay = 0,
	},
	
#ifdef CONFIG_ZTE_CAMERA_NX506J
	{
		.seq_type = SENSOR_GPIO,
		.seq_val = SENSOR_GPIO_VIO,
		.config_val = GPIO_OUT_LOW,
		.delay = 1,
	},
	{
		.seq_type = SENSOR_GPIO,
		.seq_val = SENSOR_GPIO_VIO,
		.config_val = GPIO_OUT_HIGH,
		.delay = 1,
	},
#endif
	#if 0
	{
		.seq_type = SENSOR_VREG,
		.seq_val = CAM_VAF,
		.config_val = 0,
		.delay = 0,
	},
	#endif
	{
		.seq_type = SENSOR_GPIO,
		.seq_val = SENSOR_GPIO_RESET,
		.config_val = GPIO_OUT_LOW,
		.delay = 1,
	},
	{
		.seq_type = SENSOR_GPIO,
		.seq_val = SENSOR_GPIO_RESET,
		.config_val = GPIO_OUT_HIGH,
		.delay = 30,
	},
	{
		.seq_type = SENSOR_GPIO,
		.seq_val = SENSOR_GPIO_STANDBY,
		.config_val = GPIO_OUT_LOW,
		.delay = 1,
	},
	{
		.seq_type = SENSOR_GPIO,
		.seq_val = SENSOR_GPIO_STANDBY,
		.config_val = GPIO_OUT_HIGH,
		.delay = 30,
	},
	{
		.seq_type = SENSOR_CLK,
		.seq_val = SENSOR_CAM_MCLK,
		.config_val = 24000000,
		.delay = 1,
	},
	{
		.seq_type = SENSOR_I2C_MUX,
		.seq_val = 0,
		.config_val = 0,
		.delay = 0,
	},
};

static struct v4l2_subdev_info imx214_ois_sharp_subdev_info[] = {
	{
		.code = V4L2_MBUS_FMT_SBGGR10_1X10,
		.colorspace = V4L2_COLORSPACE_JPEG,
		.fmt = 1,
		.order = 0,
	},
};

static const struct i2c_device_id imx214_ois_sharp_i2c_id[] = {
	{IMX214_OIS_SHARP_SENSOR_NAME, (kernel_ulong_t)&imx214_ois_sharp_s_ctrl},
	{ }
};

static int32_t msm_imx214_ois_sharp_i2c_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	return msm_sensor_i2c_probe(client, id, &imx214_ois_sharp_s_ctrl);
}

static struct i2c_driver imx214_ois_sharp_i2c_driver = {
	.id_table = imx214_ois_sharp_i2c_id,
	.probe  = msm_imx214_ois_sharp_i2c_probe,
	.driver = {
		.name = IMX214_OIS_SHARP_SENSOR_NAME,
	},
};

static struct msm_camera_i2c_client imx214_ois_sharp_sensor_i2c_client = {
	.addr_type = MSM_CAMERA_I2C_WORD_ADDR,
};

static const struct of_device_id imx214_ois_sharp_dt_match[] = {
	{.compatible = "qcom,imx214_ois_sharp", .data = &imx214_ois_sharp_s_ctrl},
	{}
};

MODULE_DEVICE_TABLE(of, imx214_ois_sharp_dt_match);

static struct platform_driver imx214_ois_sharp_platform_driver = {
	.driver = {
		.name = "qcom,imx214_ois_sharp",
		.owner = THIS_MODULE,
		.of_match_table = imx214_ois_sharp_dt_match,
	},
};

static int32_t imx214_ois_sharp_platform_probe(struct platform_device *pdev)
{
	int32_t rc = 0;
	const struct of_device_id *match;
	match = of_match_device(imx214_ois_sharp_dt_match, &pdev->dev);
	rc = msm_sensor_platform_probe(pdev, match->data);
	return rc;
}

static int __init imx214_ois_sharp_init_module(void)
{
	int32_t rc = 0;
	pr_info("%s:%d\n", __func__, __LINE__);
	printk("kwang imx214_ois_sharp_init_module\n");
	rc = platform_driver_probe(&imx214_ois_sharp_platform_driver,
		imx214_ois_sharp_platform_probe);
	printk("kwang imx214_ois_sharp_init_module 000 rc =%d\n",rc);
	if (!rc)
		return rc;
	printk("kwang imx214_ois_sharp_init_module 111 rc =%d\n",rc);
	pr_err("%s:%d rc %d\n", __func__, __LINE__, rc);
	return i2c_add_driver(&imx214_ois_sharp_i2c_driver);
}

static void __exit imx214_ois_sharp_exit_module(void)
{
	pr_info("%s:%d\n", __func__, __LINE__);
	if (imx214_ois_sharp_s_ctrl.pdev) {
		msm_sensor_free_sensor_data(&imx214_ois_sharp_s_ctrl);
		platform_driver_unregister(&imx214_ois_sharp_platform_driver);
	} else
		i2c_del_driver(&imx214_ois_sharp_i2c_driver);
	return;
}

static struct msm_sensor_ctrl_t imx214_ois_sharp_s_ctrl = {
	.sensor_i2c_client = &imx214_ois_sharp_sensor_i2c_client,
	.power_setting_array.power_setting = imx214_ois_sharp_power_setting,
	.power_setting_array.size = ARRAY_SIZE(imx214_ois_sharp_power_setting),
	.zte_otp_enable = true,
	.msm_sensor_mutex = &imx214_ois_sharp_mut,
	.sensor_v4l2_subdev_info = imx214_ois_sharp_subdev_info,
	.sensor_v4l2_subdev_info_size = ARRAY_SIZE(imx214_ois_sharp_subdev_info),
};

module_init(imx214_ois_sharp_init_module);
module_exit(imx214_ois_sharp_exit_module);
MODULE_DESCRIPTION("imx214_ois_sharp");
MODULE_LICENSE("GPL v2");
