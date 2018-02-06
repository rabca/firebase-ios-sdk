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

#include "Firestore/core/src/firebase/firestore/model/document_key.h"

#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

namespace firebase {
namespace firestore {
namespace model {

TEST(DocumentKey, Constructor_Empty) {
  const DocumentKey default_key;
  EXPECT_TRUE(default_key.path().empty());

  const auto& empty_key = DocumentKey::Empty();
  const auto& another_empty_key = DocumentKey::Empty();
  EXPECT_EQ(default_key, empty_key);
  EXPECT_EQ(empty_key, another_empty_key);
  EXPECT_EQ(&empty_key, &another_empty_key);
}

TEST(DocumentKey, Constructor_FromPath) {
  ResourcePath path{"rooms", "firestore", "messages", "1"};
  const DocumentKey key_from_path_copy{path};
  // path shouldn't have been moved from.
  EXPECT_FALSE(path.empty());
  EXPECT_EQ(key_from_path_copy.path(), path);

  const DocumentKey key_from_moved_path{std::move(path)};
  EXPECT_TRUE(path.empty());
  EXPECT_FALSE(key_from_moved_path.path().empty());
  EXPECT_EQ(key_from_path_copy.path(), key_from_moved_path.path());
}

TEST(DocumentKey, CopyAndMove) {
  DocumentKey key({"rooms", "firestore", "messages", "1"});
  const std::string path_string = "rooms/firestore/messages/1";
  EXPECT_EQ(path_string, key.path().CanonicalString());

  auto copied = key;
  EXPECT_EQ(path_string, copied.path().CanonicalString());
  EXPECT_EQ(key, copied);

  const auto moved = std::move(key);
  EXPECT_EQ(path_string, moved.path().CanonicalString());
  EXPECT_NE(key, moved);
  EXPECT_TRUE(key.path().empty());

  // Reassignment.

  key = copied;
  EXPECT_EQ(copied, key);
  EXPECT_EQ(path_string, key.path().CanonicalString());

  key = {};
  EXPECT_TRUE(key.path().empty());
  key = std::move(copied);
  EXPECT_NE(copied, key);
  EXPECT_TRUE(copied.path().empty());
  EXPECT_EQ(path_string, key.path().CanonicalString());
}

TEST(DocumentKey, Constructor_StaticFactory) {
  const auto key_from_segments =
      DocumentKey::FromSegments({"rooms", "firestore", "messages", "1"});
  const std::string path_string = "rooms/firestore/messages/1";
  const auto key_from_string = DocumentKey::FromPathString(path_string);
  EXPECT_EQ(path_string, key_from_string.path().CanonicalString());
  EXPECT_EQ(path_string, key_from_segments.path().CanonicalString());
  EXPECT_EQ(key_from_segments, key_from_string);
}

TEST(DocumentKey, Constructor_BadArguments) {
  ASSERT_DEATH_IF_SUPPORTED(DocumentKey(ResourcePath{"foo"}), "");
  ASSERT_DEATH_IF_SUPPORTED(DocumentKey(ResourcePath{"foo", "bar", "baz"}), "");

  ASSERT_DEATH_IF_SUPPORTED(DocumentKey::FromSegments({"foo"}), "");
  ASSERT_DEATH_IF_SUPPORTED(DocumentKey::FromSegments({"foo", "bar", "baz"}),
                            "");

  ASSERT_DEATH_IF_SUPPORTED(DocumentKey::FromPathString(""), "");
  ASSERT_DEATH_IF_SUPPORTED(DocumentKey::FromPathString("invalid//string"), "");
  ASSERT_DEATH_IF_SUPPORTED(DocumentKey::FromPathString("invalid/key/path"),
                            "");
}

TEST(DocumentKey, IsDocumentKey) {
  EXPECT_TRUE(DocumentKey::IsDocumentKey({}));
  EXPECT_FALSE(DocumentKey::IsDocumentKey({"foo"}));
  EXPECT_TRUE(DocumentKey::IsDocumentKey({"foo", "bar"}));
  EXPECT_FALSE(DocumentKey::IsDocumentKey({"foo", "bar", "baz"}));
}

TEST(DocumentKey, Comparison) {
  const DocumentKey abcd({"a", "b", "c", "d"});
  const DocumentKey abcd_too({"a", "b", "c", "d"});
  const DocumentKey xyzw({"x", "y", "z", "w"});
  EXPECT_EQ(abcd, abcd_too);
  EXPECT_NE(abcd, xyzw);

  const DocumentKey empty;
  const DocumentKey a({"a", "a"});
  const DocumentKey b({"b", "b"});
  const DocumentKey ab({"a", "a", "b", "b"});

  EXPECT_TRUE(empty < a);
  EXPECT_TRUE(empty <= a);
  EXPECT_TRUE(a > empty);
  EXPECT_TRUE(a >= empty);

  EXPECT_FALSE(a < a);
  EXPECT_TRUE(a <= a);
  EXPECT_FALSE(a > a);
  EXPECT_TRUE(a >= a);

  EXPECT_TRUE(a < b);
  EXPECT_TRUE(a <= b);
  EXPECT_TRUE(b > a);
  EXPECT_TRUE(b >= a);

  EXPECT_TRUE(a < ab);
  EXPECT_TRUE(a <= ab);
  EXPECT_TRUE(ab > a);
  EXPECT_TRUE(ab >= a);
}

}  // namespace model
}  // namespace firestore
}  // namespace firebase
