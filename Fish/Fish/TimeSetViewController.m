//
//  TimeSetViewController.m
//  Fish
//
//  Created by Alec Lombardo on 10/29/15.
//  Copyright © 2015 Alec Lombardo. All rights reserved.
//

#import "TimeSetViewController.h"

@interface TimeSetViewController ()

@end

@implementation TimeSetViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.tabBar = (TabBarViewController*) self.tabBarController;

    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

- (IBAction)updateButtonPressed:(id)sender {
    uint8_t hour = [self.hourField.text intValue];
    uint8_t minute = [self.minuteField.text intValue];
    uint8_t command[] = {0xff,hour,minute,0x00,0x00};
    NSData *nsData = [[NSData alloc] initWithBytes:command length:5];
    
    [self.tabBar.vc.peripheral writeValue:nsData forCharacteristic:self.tabBar.vc.txCharacteristic type:CBCharacteristicWriteWithoutResponse];
}
@end
