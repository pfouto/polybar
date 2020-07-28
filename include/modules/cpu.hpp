#pragma once

#include "settings.hpp"
#include "modules/meta/timer_module.hpp"

POLYBAR_NS

namespace modules {
  struct cpu_time {
    unsigned long long user;
    unsigned long long nice;
    unsigned long long system;
    unsigned long long idle;
    unsigned long long steal;
    unsigned long long total;
  };

  enum class cpu_state { NORMAL = 0, WARN, CRITICAL };

  using cpu_time_t = unique_ptr<cpu_time>;

  class cpu_module : public timer_module<cpu_module> {
   public:
    explicit cpu_module(const bar_settings&, string);

    bool update();
    string get_format() const;
    bool build(builder* builder, const string& tag) const;

   protected:
    bool read_values();
    float get_load(size_t core) const;

   private:
    static constexpr auto TAG_LABEL = "<label>";
    static constexpr const char* TAG_LABEL_WARN{"<label-warn>"};
    static constexpr const char* TAG_LABEL_CRITICAL{"<label-critical>"};
    static constexpr auto TAG_BAR_LOAD = "<bar-load>";
    static constexpr auto TAG_RAMP_LOAD = "<ramp-load>";
    static constexpr auto TAG_RAMP_LOAD_PER_CORE = "<ramp-coreload>";
    static constexpr const char* FORMAT_WARN{"format-warn"};
    static constexpr const char* FORMAT_CRITICAL{"format-critical"};

    int m_cpu_atwarning;
    int m_cpu_atcritical;

    progressbar_t m_barload;
    ramp_t m_rampload;
    ramp_t m_rampload_core;
    map<cpu_state, label_t> m_label;
    int m_ramp_padding;

    vector<cpu_time_t> m_cputimes;
    vector<cpu_time_t> m_cputimes_prev;

    float m_total = 0;
    vector<float> m_load;
  };
}

POLYBAR_NS_END
