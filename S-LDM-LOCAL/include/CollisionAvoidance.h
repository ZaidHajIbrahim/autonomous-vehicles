#ifndef COLLISION_AVOIDANCE_H
#define COLLISION_AVOIDANCE_H

#include <string>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "LDMmap.h"
#include "AMQPsend.h"

class CollisionAvoidance {
public:
    CollisionAvoidance();
    ~CollisionAvoidance() = default;

    void send_message(uint32_t id_vehicle_v, uint32_t id_vehicle_w, const std::string& collision_risk_type);
    void send_message(uint32_t recovered_vehicle_id, double speed_ms);
    /// Ta emot rått text‐meddelande och slå in i vår LDMMap, sedan kör kollisions‐loop
    void receive_message(ldmmap::LDMMap& ldmmap, const std::string& message);
    void handle_vehicle_data(ldmmap::LDMMap& ldmmap, uint32_t stationID);

    // --- Här lägger vi till den nya 360°–sökningen ---
    /// Samlar alla fordon inom radius_m meter runt stationID (360°)
    static void getNearbyByDistance(
        ldmmap::LDMMap& map,
        uint32_t stationID,
        long double radius_m,
        std::vector<ldmmap::LDMMap::returnedVehicleData_t>& out
    );

    bool detect_collision(
        uint64_t id_vehicle_v, uint64_t id_vehicle_w,
        long double pvx, long double pvy, long double pwx, long double pwy,
        long double svx, long double svy, long double swx, long double swy,
        long double avx, long double avy, long double awx, long double awy,
        long double v_heading, long double w_heading, long double th_heading,
        long double t2c_threshold, long double s2c_threshold,
        long double speed_ms_v, long double speed_ms_w,
        std::string laneID_v, std::string laneID_w
    );

    static long double calculate_x_component(long double magnitude, long double heading_degrees);
    static long double calculate_y_component(long double magnitude, long double heading_degrees);

    bool is_in_range(
        long double pvx, long double pvy, long double pwx, long double pwy,
        long double svx, long double svy, long double swx, long double swy,
        long double t2c_threshold
    );
    long double time_to_collision(
        long double pvx, long double pvy, long double pwx, long double pwy,
        long double svx, long double svy, long double swx, long double swy,
        long double avx, long double avy, long double awx, long double awy
    );
    long double space_to_collision(
        long double pvx, long double pvy, long double pwx, long double pwy,
        long double svx, long double svy, long double swx, long double swy,
        long double avx, long double avy, long double awx, long double awy,
        long double t2c
    );

    void tryRecovery(ldmmap::LDMMap& ldmmap, uint32_t stationID);

    std::unordered_map<uint32_t, std::pair<uint32_t, double>> m_vehicles_in_risk_state;
    std::mutex m_risk_state_mutex; 

    std::unordered_map<uint32_t, ldmmap::vehicleData_t> veh_map_;
    std::mutex veh_map_mutex_;

    std::unique_ptr<AMQPsend> producer_;
};

#endif