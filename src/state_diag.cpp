#include "state_diag.h"

input_switches status = {0};
io_switches status0 = {0};
trouble_code_one status1 = {0};
trouble_code_two status2 = {0};
trouble_code_three status3 = {0};
DIAG diag_current = DIAG::IN;
