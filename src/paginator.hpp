#pragma once

namespace runpac {

// bacled!
struct paginator
{

    paginator(int page_num, int item_count, int item_per_page = 3)
    : offset(0)
    , prev_page_num(0)
    , next_page_num(0)
    , error(false)
    {
        page_num = page_num <= 0 ? 1 : page_num;

        offset = (page_num - 1) * item_per_page;
        if (offset >= item_count) {
            error = true;
            return;
        }

        if (page_num > 1) {
            prev_page_num = page_num - 1;
        }
        if ((offset + item_per_page) < item_count) {
            next_page_num = page_num + 1;
        }
    }

    int offset;
    int prev_page_num;
    int next_page_num;
    bool error;
};


} // ns

