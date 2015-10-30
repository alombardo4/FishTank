//
//  ColorViewController.h
//  Fish
//
//  Created by Alec Lombardo on 10/29/15.
//  Copyright © 2015 Alec Lombardo. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "TabBarViewController.h"

@interface ColorViewController : UIViewController
@property TabBarViewController *tabBar;
@property (weak, nonatomic) IBOutlet UISlider *redSlider;
@property (weak, nonatomic) IBOutlet UISlider *greenSlider;
@property (weak, nonatomic) IBOutlet UISlider *blueSlider;
@property (weak, nonatomic) IBOutlet UISlider *brightSlider;
- (IBAction)sliderMoved:(id)sender;

@end
