//
//  ColorViewController.m
//  Fish
//
//  Created by Alec Lombardo on 10/29/15.
//  Copyright © 2015 Alec Lombardo. All rights reserved.
//

#import "ColorViewController.h"

@interface ColorViewController ()

@end

@implementation ColorViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.tabBar = (TabBarViewController *) self.tabBarController;
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

- (IBAction)sliderMoved:(id)sender {
    
    uint8_t command[] = {0x01,0x00,0x00,0x00,0x00};
    
    command[1] = (uint8_t)[self.redSlider value];
    command[2] = (uint8_t)[self.greenSlider value];
    command[3] = (uint8_t)[self.blueSlider value];
    command[4] = (uint8_t)[self.brightSlider value];

    NSData *nsData = [[NSData alloc] initWithBytes:command length:5];
    [self.tabBar.vc.peripheral writeValue:nsData forCharacteristic:self.tabBar.vc.txCharacteristic type:CBCharacteristicWriteWithoutResponse];
    
}
@end
