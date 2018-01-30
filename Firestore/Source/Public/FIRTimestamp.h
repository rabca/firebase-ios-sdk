/*
 * Copyright 2018 Google
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * An FIRTimestamp represents an absolute time from the backend at up to microsecond precision.
 * An FIRTimestamp is represented in terms of UTC and does not have an associated timezone.
 */
NS_SWIFT_NAME(Timestamp)
@interface FIRTimestamp : NSObject <NSCopying>

/** */
- (instancetype)init NS_UNAVAILABLE;

/**
 * Creates a new timestamp.
 *
 * @param seconds the number of seconds since epoch.
 * @param nanoseconds the number of nanoseconds after the seconds.
 */
+ (instancetype)timestampWithSeconds:(int64_t)seconds nanoseconds:(int32_t)nanoseconds;

/** Creates a new timestamp from the given date. */
+ (instancetype)timestampWithDate:(NSDate *)date;

/** Returns a new NSDate corresponding to this timestamp. This may lose precision. */
- (NSDate *)approximateDateValue;

/**
 * Represents seconds of UTC time since Unix epoch 1970-01-01T00:00:00Z.
 * Must be from 0001-01-01T00:00:00Z to 9999-12-31T23:59:59Z inclusive.
 */
@property(nonatomic, assign, readonly) int64_t seconds;

/**
 * Non-negative fractions of a second at nanosecond resolution. Negative second values with
 * fractions must still have non-negative nanos values that count forward in time.
 * Must be from 0 to 999,999,999 inclusive.
 */
@property(nonatomic, assign, readonly) int32_t nanoseconds;

@end

NS_ASSUME_NONNULL_END
