#ifndef EXT_HOMACHANNEL_H
#define EXT_HOMACHANNEL_H

#include "../coresim/channel.h"
#include <map>
#include <set>
#include <vector>

#define num_hw_prio_levels 8    // same as pFabric's limitation 
#define sampling_freq   5000    // # of new flows before doing another schedule_unschedued_priority()

class Flow;
class Host;
class Packet;
class AggChannel;

struct FlowComparator {
    bool operator() (Flow *a, Flow *b) {
        if (a->size == b->size) {
            return a->start_time > b->start_time;
        } else {
            return a->size > b->size;
        }
    }
};

/* HomaChannel: a single direction src-dst pair */
// Homa does not distinguish among different user priorities. Thus we will handle all priority traffic on the same channel
// also implements sender-side SRPT
class HomaChannel : public Channel {
    public:
        HomaChannel(uint32_t id, Host *s, Host *d, uint32_t priority, AggChannel *agg_channel);
        ~HomaChannel();

        void add_to_channel(Flow *flow) override;
        int next_flow_SRPT();
        int send_pkts() override;
        void insert_active_flow(Flow *) override;
        void remove_active_flow(Flow *) override;
        int calculate_scheduled_priority(Flow *flow) override;
        int calculate_unscheduled_priority() override;
        void get_unscheduled_offsets(std::vector<uint32_t> &vec);
        void record_flow_size(Flow* flow, bool scheduled) override;
        int get_sender_priority();
        void set_timeout(double time) override;
        void handle_timeout() override;


    private:
        int overcommitment_degree;
        int record_freq;
        std::priority_queue<Flow*, std::vector<Flow*>, FlowComparator> sender_flows;
        //std::map<Flow *, int> active_flows;            // flows with size > RTTbytes; maintained by receiver
        std::set<Flow *> active_flows;            // flows with size > RTTbytes; maintained by receiver
        std::vector<uint32_t> sampled_scheduled_flow_size;
        std::vector<uint32_t> sampled_unscheduled_flow_size;
        std::vector<uint32_t> curr_unscheduled_offsets;
        int curr_unscheduled_prio_levels;
        std::set<Flow *> sampled_scheduled_flows;
        std::set<Flow *> sampled_unscheduled_flows;

};

#endif  // EXT_HOMACHANNEL_H