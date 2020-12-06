
#include "mocks/mockfile.h"

#include "core/vfs/buffer.h"

using ::testing::_;
using ::testing::NiceMock;

static const String filename{ UTEXT("test.text") };

TEST(File, testOpen)
{   
   MockFile file;
   EXPECT_CALL(file, virOpen(filename, _));
   
   bool result = file.open(filename);
   EXPECT_TRUE(result);
}

TEST(File, testRead)
{
   NiceMock<MockFile> file;
   file.open(filename);

   auto& buffer = file.getMockBuffer();
   EXPECT_CALL(buffer, read(_, _));
   ON_CALL(buffer, read).WillByDefault([](void* pdata, int len) {
      memset(pdata, 0, len);
      ((char*)pdata)[0] = 'p';
      return 1;
   });

   char buff[128];
   int len = file.read(buff, 127);
   EXPECT_EQ(len, 1);
}
