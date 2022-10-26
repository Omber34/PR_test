//
// Created by Herman on 1/24/2022.
//

#include <cassert>
#include <fstream>
#include "FileManager.h"

char testFileData[] = "a b c d e f g h i j k l m n o p q r s t u v w x y z"
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

int main(int argc, char **argv)
{
  constexpr char testFileName1[] = "test.txt";
  std::ofstream testFile(testFileName1, std::ios::out | std::ios::binary);
  testFile.write(testFileData, sizeof(testFileData));
  testFile.close();

  auto filePacket1 = FileManager::loadFileToFilePacket(testFileName1);
  assert(filePacket1.packets.size() != 0);
  assert(strncmp(filePacket1.packets.begin()->body(), testFileData, filePacket1.packets.begin()->body_length()) == 0);

  constexpr char testFileName2[] = "test2.txt";
  FileManager::saveFilePacketToDisk(testFileName2, filePacket1);

  std::ifstream testFile2(testFileName2, std::ios::in | std::ios::binary);
  char *buffer = new char[ChatPacket::max_body_length + 1];
  testFile2.read(buffer, ChatPacket::max_body_length);

  assert(strncmp(buffer, testFileData, ChatPacket::max_body_length) == 0);

  return 0;
}