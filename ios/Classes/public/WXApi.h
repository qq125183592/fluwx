//
//  WXApi.h
//  所有Api接口
//
//  Created by Wechat on 12-2-28.
//  Copyright (c) 2012年 Tencent. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "WXApiObject.h"

NS_ASSUME_NONNULL_BEGIN

#pragma mark - WXApiDelegate
/*! @brief 接收并处理来自微信终端程序的事件消息
 *
 * 接收并处理来自微信终端程序的事件消息，期间微信界面会切换到第三方应用程序。
 * WXApiDelegate 会在handleOpenURL:delegate:中使用并触发。
 */
@protocol WXApiDelegate <NSObject>
@optional

/*! @brief 收到一个来自微信的请求，第三方应用程序处理完后调用sendResp向微信发送结果
 *
 * 收到一个来自微信的请求，异步处理完成后必须调用sendResp发送处理结果给微信。
 * 可能收到的请求有GetMessageFromWXReq、ShowMessageFromWXReq等。
 * @param req 具体请求内容，是自动释放的
 */
- (void)onReq:(BaseReq*)req;



/*! @brief 发送一个sendReq后，收到微信的回应
 *
 * 收到一个来自微信的处理结果。调用一次sendReq后会收到onResp。
 * 可能收到的处理结果有SendMessageToWXResp、SendAuthResp等。
 * @param resp具体的回应内容，是自动释放的
 */
- (void)onResp:(BaseResp*)resp;

@end

#pragma mark - WXApiLogDelegate

@protocol WXApiLogDelegate <NSObject>

- (void)onLog:(NSString*)log logLevel:(WXLogLevel)level;

@end

#pragma mark - WXApi

/*! @brief 微信Api接口函数类
 *
 * 该类封装了微信终端SDK的所有接口
 */
@interface WXApi : NSObject

/*! @brief WXApi的成员函数，向微信终端程序注册第三方应用。
 *
 * 需要在每次启动第三方应用程序时调用。第一次调用后，会在微信的可用应用列表中出现，默认开启MTA数据上报。
 * iOS7及以上系统需要调起一次微信才会出现在微信的可用应用列表中。
 * @attention 请保证在主线程中调用此函数
 * @param appid 微信开发者ID
 * @param typeFlag 应用支持打开的文件类型
 * @return 成功返回YES，失败返回NO。
 */
+ (BOOL)registerApp:(NSString *)appid;

/*! @brief WXApi的成员函数，向微信终端程序注册第三方应用。
 *
 * 需要在每次启动第三方应用程序时调用。第一次调用后，会在微信的可用应用列表中出现。
 * iOS7及以上系统需要调起一次微信才会出现在微信的可用应用列表中。
 * @attention 请保证在主线程中调用此函数
 * @param appid 微信开发者ID
 * @param isEnableMTA 是否支持MTA数据上报
 * @return 成功返回YES，失败返回NO。
 */
+ (BOOL)registerApp:(NSString *)appid enableMTA:(BOOL)isEnableMTA;


/*! @brief WXApi的成员函数，向微信终端程序注册应用支持打开的文件类型。
 *
 * 需要在每次启动第三方应用程序时调用。调用后并第一次成功分享数据到微信后，会在微信的可用应用列表中出现。
 * @see registerApp
 * @param typeFlag 应用支持打开的数据类型, enAppSupportContentFlag枚举类型 “|” 操作后结果
 */
+ (void)registerAppSupportContentFlag:(UInt64)typeFlag;



/*! @brief 处理微信通过URL启动App时传递的数据
 *
 * 需要在 application:openURL:sourceApplication:annotation:或者application:handleOpenURL中调用。
 * @param url 微信启动第三方应用时传递过来的URL
 * @param delegate  WXApiDelegate对象，用来接收微信触发的消息。
 * @return 成功返回YES，失败返回NO。
 */
+ (BOOL)handleOpenURL:(NSURL *)url delegate:(nullable id<WXApiDelegate>)delegate;



/*! @brief 检查微信是否已被用户安装
 *
 * @return 微信已安装返回YES，未安装返回NO。
 */
+ (BOOL)isWXAppInstalled;



/*! @brief 判断当前微信的版本是否支持OpenApi
 *
 * @return 支持返回YES，不支持返回NO。
 */
+ (BOOL)isWXAppSupportApi;



/*! @brief 获取微信的itunes安装地址
 *
 * @return 微信的安装地址字符串。
 */
+ (NSString *)getWXAppInstallUrl;



/*! @brief 获取当前微信SDK的版本号
 *
 * @return 返回当前微信SDK的版本号
 */
+ (NSString *)getApiVersion;



/*! @brief 打开微信
 *
 * @return 成功返回YES，失败返回NO。
 */
+ (BOOL)openWXApp;



/*! @brief 发送请求到微信，等待微信返回onResp
 *
 * 函数调用后，会切换到微信的界面。第三方应用程序等待微信返回onResp。微信在异步处理完成后一定会调用onResp。支持以下类型
 * SendAuthReq、SendMessageToWXReq。
 * @param req 具体的发送请求，在调用函数后，请自己释放。
 * @return 成功返回YES，失败返回NO。
 */
+ (BOOL)sendReq:(BaseReq*)req;

/*! @brief 发送Auth请求到微信，支持用户没安装微信，等待微信返回onResp
 *
 * 函数调用后，会切换到微信的界面。第三方应用程序等待微信返回onResp。微信在异步处理完成后一定会调用onResp。支持SendAuthReq类型。
 * @param req 具体的发送请求，在调用函数后，请自己释放。
 * @param viewController 当前界面对象。
 * @param delegate  WXApiDelegate对象，用来接收微信触发的消息。
 * @return 成功返回YES，失败返回NO。
 */
+ (BOOL)sendAuthReq:(SendAuthReq*)req viewController:(UIViewController*)viewController delegate:(nullable id<WXApiDelegate>)delegate;


/*! @brief 收到微信onReq的请求，发送对应的应答给微信，并切换到微信界面
 *
 * 函数调用后，会切换到微信的界面。第三方应用程序收到微信onReq的请求，异步处理该请求，完成后必须调用该函数。可能发送的相应有
 * GetMessageFromWXResp、ShowMessageFromWXResp等。
 * @param resp 具体的应答内容，调用函数后，请自己释放
 * @return 成功返回YES，失败返回NO。
 */
+ (BOOL)sendResp:(BaseResp*)resp;


/*! @brief WXApi的成员函数，接受微信的log信息。byBlock
    注意1:SDK会强引用这个block,注意不要导致内存泄漏,注意不要导致内存泄漏
    注意2:调用过一次startLog by block之后，如果再调用一次任意方式的startLoad,会释放上一次logBlock，不再回调上一个logBlock
 *
 *  @param level 打印log的级别
 *  @param logBlock 打印log的回调block
 */
+ (void)startLogByLevel:(WXLogLevel)level logBlock:(WXLogBolock)logBlock;

/*! @brief WXApi的成员函数，接受微信的log信息。byDelegate 
    注意1:sdk会弱引用这个delegate，这里可加任意对象为代理，不需要与WXApiDelegate同一个对象
    注意2:调用过一次startLog by delegate之后，再调用一次任意方式的startLoad,不会再回调上一个logDelegate对象
 *  @param level 打印log的级别
 *  @param logDelegate 打印log的回调代理，
 */
+ (void)startLogByLevel:(WXLogLevel)level logDelegate:(id<WXApiLogDelegate>)logDelegate;

/*! @brief 停止打印log，会清理block或者delegate为空，释放block
 *  @param 
 */
+ (void)stopLog;
@end

NS_ASSUME_NONNULL_END
