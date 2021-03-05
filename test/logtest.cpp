#include <iostream>

#include "log/logmanager.hpp"
#include "server/smartdb.hpp"
#include "file/page.hpp"
#include "gtest/gtest.h"

namespace smartdb {
  void print_log_records(log_manager* lM, std::string msg) {
    std::cout << msg << std::endl;
    log_manager::log_iterator iter = lM->iterator();
    while (iter.has_next()) {
      std::vector<char> rec = iter.next();
      page p(rec);
      std::string s = p.get_string(0);
      int npos = page::max_length(s.size());
      int val = p.get_int(npos);
      std::cout << "[" << s << ", " << val << "]";
    }
    std::cout << std::endl;
  }
  
  std::vector<char> create_log_record(std::string s, int n) {
    int spos = 0;
    int npos = spos + page::max_length(s.size());
    std::vector<char> b(npos+sizeof(int));
    page p(b);
    p.set_string(spos, s);
    p.set_int(npos, n);
    b = p.contents();
    return b;
  }

  void create_records(log_manager* lM, int start, int end) {
    std::cout << "Creating records: ";
    for (int i = start; i <= end; i++) {
      std::vector<char> rec = create_log_record("record"+std::to_string(i), i+100);
      int lsn = lM->append(rec);
      std::cout << lsn << " ";
    }
    std::cout << std::endl;
  }
    
  TEST(log, logtest) {
    smartdb db("logtest", 400, 8);
    log_manager* lM = db.new_lm();
    print_log_records(lM, "The initial empty log file: ");
    std::cout << "done" << std::endl;
    create_records(lM, 1, 35);
    print_log_records(lM, "The log file now has these records:");
    create_records(lM, 36, 70);
    lM->flush(65);
    print_log_records(lM, "The log file now has these records:");
  }
}


