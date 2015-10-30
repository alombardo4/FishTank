//
//  TimeSetViewController.h
//  Fish
//
//  Created by Alec Lombardo on 10/29/15.
//  Copyright © 2015 Alec Lombardo. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "TabBarViewController.h"

@interface TimeSetViewController : UIViewController
@property (weak, nonatomic) IBOutlet UITextField *hourField;
@property (weak, nonatomic) IBOutlet UITextField *minuteField;
- (IBAction)updateButtonPressed:(id)sender;
@property TabBarViewController* tabBar;
@end
