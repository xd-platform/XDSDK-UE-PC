# Changelog

## 6.5.1

### New Feature
* 对象`XUError`新增`GetExtraString`接口，与移动端保持统一

### Breaking Changes
* 谷歌本地配置文件初始化字段`CLIENT_ID`改为`CLIENT_ID_FOR_PC`
* 国内支付和国外支付API合并为新的接口



## 6.5.0

### New Feature
* 新增海外谷歌登录
* 新增国内扫码支付
* 新增注销账户功能
* 新增国内TapTap登录
* 新增国内客服中心
* 新增国内用户中心
* 新增越南语支持

### Breaking Changes
* 初始化方式更新，支持本地Json文件初始化

### Optimization
* 协议展示优化
* Ip地址支持异步获取
* TapSDK部分优化

### Fixed bugs
* 多语言Json文件加载失败引发的崩溃
* 应用关闭时，本地存储引发的崩溃
* 其他一些偶发的崩溃

## 6.3.1

### New Feature
* TapMoment配置APPID国内国外设置统一

### Optimization
* TapDB 优化更新


## 6.3.0

### New Feature
* TapSDK更新到3.9.0
* 新增海外TapTap扫码登录
* 新增海外TapTap网页登录
* 新增海外网页支付
* 新增海外客服中心
* 新增海外用户中心
* 新增检查补款的功能

