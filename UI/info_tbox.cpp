#include "ui/ui.hpp"
#include "if.hpp"

info_tbox::info_tbox()
{
    tbox = NULL;
    data_store = NULL;
    type = OTHER;
}

void info_tbox::setdata()
{
    char *fstr;
    switch(type)
    {
        case INT:
        {
            fstr = "%d";
            break;
        }
        case LONG:
        {
            fstr = "%ld";
            break;
        }
        case STRING:
        case PASSWORD:
        {
            char *s = (char *) data_store;
            strcpy(s, tbox->getstr());
            return;
        }
        case CHAR:
        {
            fstr = "%c";
            break;
        }
        case DOUBLE:
        {
            fstr = "%g";
            break;
        }
        case FLOAT:
        {
            fstr = "%f";
            break;
        }
        default:
            return;
    }

    sscanf(tbox->getstr(), fstr, data_store);
}
