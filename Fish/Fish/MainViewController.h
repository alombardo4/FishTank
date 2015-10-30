//
//  MainViewController.h
//  Fish
//
//  Created by Alec Lombardo on 10/29/15.
//  Copyright © 2015 Alec Lombardo. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "TabBarViewController.h"

@interface MainViewController : UIViewController
- (IBAction)standardPressed:(id)sender;
- (IBAction)offPressed:(id)sender;
- (IBAction)whitePressed:(id)sender;
@property TabBarViewController* tabBar;

@end
