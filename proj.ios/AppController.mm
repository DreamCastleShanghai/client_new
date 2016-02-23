#import "AppController.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#import "FNoticeManager.h"
#import "FDataManager.h"

@implementation AppController

#pragma mark -
#pragma mark Application lifecycle

// CrossApp application instance
static AppDelegate s_sharedApplication;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    
    // Override point for customization after application launch.

    // Add the view controller's view to the window and display.
    
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    
    // Init the EAGLView

    // Use RootViewController manage EAGLView 
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = NO;

    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:viewController];
    }
    
    [window makeKeyAndVisible];
    
    [[UIApplication sharedApplication] setStatusBarHidden:NO];
    
    if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0) {
        
        [[UIApplication sharedApplication]registerUserNotificationSettings:[UIUserNotificationSettings
                                                                            settingsForTypes:(UIUserNotificationTypeSound|UIUserNotificationTypeAlert|UIUserNotificationTypeBadge)
                                                                            categories:nil]];
        [[UIApplication sharedApplication]registerForRemoteNotifications];
        
    } else {
        
        UIRemoteNotificationType myTypes = UIRemoteNotificationTypeBadge|UIRemoteNotificationTypeAlert|UIRemoteNotificationTypeSound;
        
        [[UIApplication sharedApplication] registerForRemoteNotificationTypes:myTypes];
    }
    
    CrossApp::CCApplication::sharedApplication()->run();
        
    
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    CrossApp::CAApplication::getApplication()->pause();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    CrossApp::CAApplication::getApplication()->resume();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    CrossApp::CCApplication::sharedApplication()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    CrossApp::CCApplication::sharedApplication()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
{
    //CrossApp::CCApplication::sharedApplication()->didReceiveRemoteNotification();
    NSString *id = [userInfo objectForKey:@"id"];
    NSString *type = [userInfo objectForKey:@"tp"];
    NSString *title = [userInfo objectForKey:@"title"];
    NSString *content = [[[userInfo objectForKey:@"aps"] objectForKey:@"alert"] objectForKey:@"body"];
    
    std::string strTitle = [title UTF8String];
    std::string strDetail = [content UTF8String];
    FNoticeManager::sharedFNoticeManager()->playNoticeVoice();
    FNoticeManager::sharedFNoticeManager()->addNotice([id intValue], [type intValue], strTitle, strDetail, 0, 0, true);
    NSLog(@"didReceiveRemoteNotification %@ %@ %@", id, type,content);
    
    UIApplication *app = [UIApplication sharedApplication];
    NSArray *localArr = [app scheduledLocalNotifications];
    UILocalNotification *localNotice = nil;
    if (localArr) {
        for (UILocalNotification *noti in localArr) {
            NSDictionary *dict = noti.userInfo;
            if (dict) {
                NSString *inKey = [dict objectForKey:@"key"];
                NSLog(@"notice %@", inKey);
                if ([inKey isEqualToString:id]) {
                    if (localNotice){
                        [localNotice release];
                        localNotice = nil;
                    }
                    localNotice = [noti retain];
                    break;
                }
            }
        }
        
        if (localNotice) {
            //不推送 取消推送
            [app cancelLocalNotification:localNotice];
            [localNotice release];
            return;
        }
    }
}

- (void)application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification
{
    FNoticeManager::sharedFNoticeManager()->playNoticeVoice();
    //CrossApp::CCApplication::sharedApplication()->didReceiveLocalNotification();
    NSDictionary *dict = notification.userInfo;
    if (dict) {
        NSString *inKey = [dict objectForKey:@"key"];
        NSLog(@"My token is: %@", inKey);
    }
    NSLog(@"didReceiveLocalNotification");
}

- (void)application:(UIApplication*)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData*)deviceToken
{
    NSString *deviceTokenStr = [NSString stringWithFormat:@"%@",deviceToken];
    deviceTokenStr = [[deviceTokenStr substringWithRange:NSMakeRange(0, 72)] substringWithRange:NSMakeRange(1, 71)];
    deviceTokenStr = [deviceTokenStr stringByReplacingOccurrencesOfString:@" " withString:@""];
    FNoticeManager::sharedFNoticeManager()->sendNoticeToken((unsigned char*)[deviceTokenStr UTF8String]);
    FDataManager::getInstance()->setUserToken(crossapp_format_string("%s", (unsigned char*)[deviceTokenStr UTF8String]));
    NSLog(@"My token is: %@", deviceToken);
}

- (void)application:(UIApplication*)application didFailToRegisterForRemoteNotificationsWithError:(NSError*)error
{
    NSLog(@"Failed to get token, error: %@", error);
}

#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


- (void)dealloc {
    [window release];
    [super dealloc];
}


@end
