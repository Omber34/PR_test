//
// Created by Herman on 4/17/2022.
//

#ifndef PR_TEST_FILEMANAGERTEST_H
#define PR_TEST_FILEMANAGERTEST_H

#include "gtest/gtest.h"
#include "../client/ClientFileManager.h"
#include "QObjectTestHelper.h"
#include <fstream>

class FileManagerTest : public testing::Test {
protected:
    void SetUp() override {
        testTxtFileDataSize = strlen(testTxtFileData);
    }

    void TearDown() override {
    }

    const char* testTxtFileData =
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z";

    size_t testTxtFileDataSize;
};

#endif //PR_TEST_FILEMANAGERTEST_H
