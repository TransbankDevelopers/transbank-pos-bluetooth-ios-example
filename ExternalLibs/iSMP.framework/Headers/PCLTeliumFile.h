//
//  PCLTeliumFile.h
//  iSMP
//
//  Created by Jeremy PELTIER (Ingenico Partner) on 16/01/2018.
//  Copyright © 2018 Ingenico. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface PCLTeliumFile : NSObject

@property (strong, nonatomic) NSString *name;
@property (strong, nonatomic) NSString *path;
@property (strong, nonatomic) NSString *type;
@property (assign, nonatomic) BOOL isDirectory;
@property (assign, nonatomic) unsigned int size;

-(instancetype)initWithName:(NSString *)name path:(NSString *)path isDirectory:(BOOL)isDirectory size:(unsigned int)size;

@end
