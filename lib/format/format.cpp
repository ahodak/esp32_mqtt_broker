#include "format.hpp"

String format_zero_padding(int value, int width)
{
    String s(value);
    while (s.length() < width)
        s = "0" + s;

    return s;
}

String format_value(double value)
{
    if (value == 0.0)
        return "0";

    if (value < 0)
        return "-" + format_value(-value);

    // No decimal places
    if (value >= 1)
        return String(value, 0);
    if (value < 0.00001)
        return String(value, 6);
    if (value < 0.0001)
        return String(value, 5);
    if (value < 0.001)
        return String(value, 4);
    if (value < 0.01)
        return String(value, 3);
    if (value < 0.1)
        return String(value, 2);
    return String(value, 1);
}

String format_si(double value, int decimal_places)
{
    if (value == 0.0)
        return "0";

    if (value < 0)
        return "-" + format_si(-value, decimal_places);

    auto value_abs = fabs(value);
    if (value_abs < 1E-9)
        return String(value * 1E12, decimal_places) + "p";
    if (value_abs < 1E-6)
        return String(value * 1E9, decimal_places) + "n";
    if (value_abs < 1E-3)
        return String(value * 1E6, decimal_places) + "u";
    if (value_abs < 1)
        return String(value * 1E3, decimal_places) + "m";
    if (value_abs < 1E3)
        return String(value, decimal_places);
    if (value_abs < 1E6)
        return String(value / 1E3, decimal_places) + "k";
    if (value_abs < 1E9)
        return String(value / 1E6, decimal_places) + "M";
    if (value_abs < 1E12)
        return String(value / 1E9, decimal_places) + "G";
    if (value_abs < 1E15)
        return String(value / 1E12, decimal_places) + "T";

    return "NaN";
}

String format_memory(size_t bytes, int decimal_places)
{
    const char *suffix[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    auto val = (double)bytes;
    auto suffix_index = 0;
    while (val >= 1024.0)
    {
        val /= 1024;
        suffix_index++;
    }

    return String(val, decimal_places) + " " + suffix[suffix_index];
}

String format_duration(time_t seconds)
{
  auto days = seconds / (60 * 60 * 24);
  auto tm = gmtime(&seconds);
  String duration = days > 0 ? String(days) + " days, " : "";
  char time_buff[9];
  strftime(time_buff, 9, "%H:%M:%S", tm);
  return duration + time_buff;
}
