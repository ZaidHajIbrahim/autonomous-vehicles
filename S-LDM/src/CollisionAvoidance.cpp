#include <cmath>
#include <algorithm>
#include <iostream>
#include <mutex>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <iomanip>
#include "../include/AMQPclient.h"
#include "../include/AMQPsend.h"
#include "../include/CollisionAvoidance.h"
#include "../include/utils.h"
#include "../include/LDMmap.h"
#include "../decoder-module/asn1/include/DENM.h"
#include "../include/AMQPrecv.h"
// Constants for conversion
constexpr double DEG_TO_RAD = M_PI / 180.0;
// constexpr double OUTDATED_THRESHOLD_S = 3 * 1e6;
constexpr double OUTDATED_THRESHOLD_S = 3.0; // 3 seconds


/*
Optimal values allow for the adjustment of false negatives and false positives
and are set here (to, respectively, 4.8 s and 4.2 m) for an urban scenario with
speed limits set at 50 km/h (13.89 m/s).
*/
constexpr double HEADING_THRESHOLD = 15;      // 15 degrees
constexpr double HEADING_THRESHOLD_CROSS = 165;
// constexpr double T2C_THRESHOLD = 4.8 * 1e6;
constexpr double T2C_THRESHOLD = 4.8;     // 4.8 seconds
constexpr double S2C_THRESHOLD = 4.2;           // 4.2 meters

// Parameters for dynamic detection radius
constexpr long double BASE_RADIUS = 5.0;         // R0, meters
constexpr long double LINEAR_FACTOR = 0.1;       // k, meters per km/h
constexpr long double QUADRATIC_FACTOR = 0.01;   // c, meters per (km/h)^2

// Define static members
// std::unordered_map<uint32_t, std::pair<double, double>> CollisionAvoidance::previous_data;
// std::mutex CollisionAvoidance::data_mutex;


CollisionAvoidance::CollisionAvoidance()
{
 producer_ = std::make_unique<AMQPsend>(
   "127.0.0.1:5672",
   "topic://sldm.to.van3t"
 );
}


// Calculate the x and y components of velocity or acceleration
long double CollisionAvoidance::calculate_x_component(long double magnitude, long double heading_degrees) {
   long double heading_radians = heading_degrees * DEG_TO_RAD;
   return magnitude * std::cos(heading_radians);
}

long double CollisionAvoidance::calculate_y_component(long double magnitude, long double heading_degrees) {
   long double heading_radians = heading_degrees * DEG_TO_RAD;
   return magnitude * std::sin(heading_radians);
}


void CollisionAvoidance::handle_vehicle_data(ldmmap::LDMMap& ldmmap, uint32_t stationID) {
   // double t_current_last = get_timestamp_us(); // Get the current time in microseconds


   // Retrieve the data for the current vehicle
   ldmmap::LDMMap::returnedVehicleData_t currentVehicle;
   if (ldmmap.lookup(stationID, currentVehicle) != ldmmap::LDMMap::LDMMAP_OK) {
       std::cerr << "[ERROR] Vehicle with stationID " << stationID << " not found in LDMMap." << std::endl;
       return;
   }


   {
       std::lock_guard<std::mutex> lock(m_risk_state_mutex);
       auto it_risk_state = m_vehicles_in_risk_state.find(stationID);
       if (it_risk_state != m_vehicles_in_risk_state.end()) {
           uint32_t id_vehicle_v_it_was_risky_to = it_risk_state->second.first;
           double original_speed_before_risk = it_risk_state->second.second;


        //    ldmmap::LDMMap::returnedVehicleData_t otherVehicle;
        //    if (ldmmap.lookup(id_vehicle_v_it_was_risky_to, otherVehicle) == ldmmap::LDMMap::LDMMAP_OK) {


        //        long double pvx = currentVehicle.vehData.lat;
        //        long double pvy = currentVehicle.vehData.lon;
        //        long double pwx = otherVehicle.vehData.lat;
        //        long double pwy = otherVehicle.vehData.lon;


        //        long double svx = calculate_x_component(currentVehicle.vehData.speed_ms, currentVehicle.vehData.heading);
        //        long double svy = calculate_y_component(currentVehicle.vehData.speed_ms, currentVehicle.vehData.heading);


        //        long double accel1 = currentVehicle.vehData.acceleration_ms;
        //        long double avx = calculate_x_component(accel1, currentVehicle.vehData.heading);
        //        long double avy = calculate_y_component(accel1, currentVehicle.vehData.heading);


        //        long double swx = calculate_x_component(otherVehicle.vehData.speed_ms, otherVehicle.vehData.heading);
        //        long double swy = calculate_y_component(otherVehicle.vehData.speed_ms, otherVehicle.vehData.heading);


        //        long double accel2 = otherVehicle.vehData.acceleration_ms;
        //        long double awx = calculate_x_component(accel2, otherVehicle.vehData.heading);
        //        long double awy = calculate_y_component(accel2, otherVehicle.vehData.heading);

              
        //        long double t2c = time_to_collision(
        //            pvx, pvy, pwx, pwy,
        //            svx, svy, swx, swy,
        //            avx, avy, awx, awy
        //        );
        //        long double s2c = space_to_collision(
        //            pvx, pvy, pwx, pwy,
        //            svx, svy, swx, swy,
        //            avx, avy, awx, awy,
        //            t2c
        //        );


        //        std::cout << "[RECOVERY] t2c=" << t2c
        //                  << ", s2c=" << s2c << " (th=" << S2C_THRESHOLD << ")" << std::endl;
        //        /* Återhämta när ingen framtida kollision (t2c<0)
        //           eller s2c ≥ statisk tröskel */
        //        if (t2c < 0 || s2c >= S2C_THRESHOLD) {
        //            std::cout << "[RECOVERED] Vehicle " << stationID
        //                      << " recovered (s2c=" << s2c << " ≥ " << S2C_THRESHOLD << ")"
        //                      << std::endl;
        //            send_message(stationID, original_speed_before_risk);
        //            m_vehicles_in_risk_state.erase(it_risk_state);
        //        }
        //    } else { // Om det andra fordonet inte längre finns i LDM
        //         std::cout << "[RECOVERED_BY_ABSENCE] Vehicle " << stationID
        //                   << " target vehicle " << id_vehicle_v_it_was_risky_to << " not in LDM. Assuming recovery." << std::endl;
        //        m_vehicles_in_risk_state.erase(it_risk_state);
        //    }
       }
   }
//    --- End of recovery check ---


   // Calculate velocity for the current vehicle
   long double svx = calculate_x_component(currentVehicle.vehData.speed_ms, currentVehicle.vehData.heading);
   long double svy = calculate_y_component(currentVehicle.vehData.speed_ms, currentVehicle.vehData.heading);


   // Calculate acceleration for the current vehicle
   long double accel1 = currentVehicle.vehData.acceleration_ms;
   long double avx = calculate_x_component(accel1, currentVehicle.vehData.heading);
   long double avy = calculate_y_component(accel1, currentVehicle.vehData.heading);


   // Debug print for current vehicle acceleration components
   std::cout << "[DEBUG] Current Vehicle (ID: " << currentVehicle.vehData.stationID << ") Velocity Components: "
             << "svx: " << svx << ", svy: " << svy << " Acceleration Components: "
             << "avx: " << avx << ", avy: " << avy << ", Raw Accel: " << accel1 << std::endl;     


   // Calculate realistic stopping distance (reaction + braking) for current speed
   // Example: stopping_distance = reaction_distance + braking_distance
   // For 12 m/s (~43 km/h), interpolate from your table or use a formula

//    long double stopping_distance = 1.0 * currentVehicle.vehData.speed_ms * 1.0 // reaction time ~1s
//        + std::pow(currentVehicle.vehData.speed_ms, 2) / (2 * 7.0); // braking, a ≈ 7 m/s^2 dry asphalt

//    Calculate dynamic detection radius based on speed (convert m/s to km/h)
//    long double speed_kmh = currentVehicle.vehData.speed_ms * 3.6;
//    long double radius = BASE_RADIUS
//                       + LINEAR_FACTOR * speed_kmh
//                       + QUADRATIC_FACTOR * speed_kmh * speed_kmh;

//    std::cout << "[DEBUG] Dynamic radius for nearby vehicles: " << radius << " m (speed: " << speed_kmh << " km/h)" << std::endl;
 long double radius = currentVehicle.vehData.speed_ms * T2C_THRESHOLD;
std::cout << "[DEBUG] Dynamic radius for T2C: " << radius << " m (speed: " << currentVehicle.vehData.speed_ms << " m/s, T2C: " << T2C_THRESHOLD << " s)" << std::endl;

// Convert lat/lon to meters relative to a reference point (e.g., currentVehicle)
constexpr double EARTH_RADIUS_M = 6371000.0;
double lat0_rad = currentVehicle.vehData.lat * DEG_TO_RAD;
double m_per_deg_lat = 111320.0;
double m_per_deg_lon = std::cos(lat0_rad) * 111320.0;

auto latlon_to_meters = [&](double lat, double lon) {
    double dx = (lat - currentVehicle.vehData.lat) * m_per_deg_lat;
    double dy = (lon - currentVehicle.vehData.lon) * m_per_deg_lon;
    return std::make_pair(dx, dy);
};

   // Retrieve nearby vehicles within a certain radius
   std::vector<ldmmap::LDMMap::returnedVehicleData_t> nearbyVehicles;
   if (ldmmap.rangeSelect(radius, stationID, nearbyVehicles) != ldmmap::LDMMap::LDMMAP_OK) {
       std::cerr << "[ERROR] Failed to retrieve nearby vehicles for stationID " << stationID << std::endl;
       return;
   }   


   // Process each nearby vehiclez
   for (const auto& otherVehicle : nearbyVehicles) {
       if (otherVehicle.vehData.stationID == stationID) continue;

       // For current vehicle
       auto [pvx, pvy] = latlon_to_meters(currentVehicle.vehData.lat, currentVehicle.vehData.lon);
    //    // For other vehicle
       auto [pwx, pwy] = latlon_to_meters(otherVehicle.vehData.lat, otherVehicle.vehData.lon);
    // long double pvx = currentVehicle.vehData.lat;
    // long double pvy = currentVehicle.vehData.lon;
    // long double pwx = otherVehicle.vehData.lat;
    // long double pwy = otherVehicle.vehData.lon;
       // Debug print for nearby vehicle data
       std::cout << "[DEBUG] Nearby Vehicle (ID: " << otherVehicle.vehData.stationID << "): "
                 << "Lat: " << otherVehicle.vehData.lat << ", Lon: " << otherVehicle.vehData.lon
                 << ", Heading: " << otherVehicle.vehData.heading << ", Speed: " << otherVehicle.vehData.speed_ms
                 << ", Accel: " << otherVehicle.vehData.acceleration_ms
                 << ", Timestamp: " << otherVehicle.vehData.timestamp_us << std::endl;


       // Calculate velocity for the nearby vehicle
       long double swx = calculate_x_component(otherVehicle.vehData.speed_ms, otherVehicle.vehData.heading);
       long double swy = calculate_y_component(otherVehicle.vehData.speed_ms, otherVehicle.vehData.heading);

       // Calculate acceleration for the other vehicle
       long double accel2 = otherVehicle.vehData.acceleration_ms;
       long double awx = calculate_x_component(accel2, otherVehicle.vehData.heading);
       long double awy = calculate_y_component(accel2, otherVehicle.vehData.heading);


       // Debug print for current vehicle acceleration components
       std::cout << "[DEBUG] Current Vehicle (ID: " << otherVehicle.vehData.stationID << ") Velocity Components: "
       << "svx: " << svx << ", svy: " << svy << " Acceleration Components: "
       << "avx: " << avx << ", avy: " << avy << std::endl;

       long double t2c = time_to_collision(pvx, pvy, pwx, pwy, svx, svy, swx, swy, avx, avy, awx, awy);
        long double s2c = space_to_collision(pvx, pvy, pwx, pwy, svx, svy, swx, swy, avx, avy, awx, awy, t2c);
        std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXX t2c " << t2c << " s2c " << s2c << std::endl;


       // Run the collision detection algorithm
       if (detect_collision(
           currentVehicle.vehData.stationID, otherVehicle.vehData.stationID,
           pvx, pvy, pwx, pwy,
           svx, svy, swx, swy,
           avx, avy, awx, awy,
           currentVehicle.vehData.heading, otherVehicle.vehData.heading, HEADING_THRESHOLD,
           T2C_THRESHOLD, S2C_THRESHOLD,


           currentVehicle.vehData.speed_ms, otherVehicle.vehData.speed_ms,
           currentVehicle.vehData.laneID, otherVehicle.vehData.laneID
       ))
       {
           std::lock_guard<std::mutex> lock(m_risk_state_mutex);
           if (m_vehicles_in_risk_state.find(currentVehicle.vehData.stationID) == m_vehicles_in_risk_state.end()) {
               std::cout << "[RISK_STATE_SET] Vehicle " << currentVehicle.vehData.stationID
                         << " (ego, 'v') is now in risk state targeting " << otherVehicle.vehData.stationID
                         << " ('w'). Original speed of ego: " << currentVehicle.vehData.speed_ms << std::endl;
               m_vehicles_in_risk_state[currentVehicle.vehData.stationID] =
                   {otherVehicle.vehData.stationID, currentVehicle.vehData.speed_ms};
           } else {
               std::cout << "[RISK_STATE_INFO] Vehicle " << currentVehicle.vehData.stationID
                         << " was already in a risk state. Not overriding." << std::endl;
           }
       }
   }
}


bool CollisionAvoidance::detect_collision(
   uint64_t id_vehicle_v, uint64_t id_vehicle_w,
   long double pvx, long double pvy, long double pwx, long double pwy,
   long double svx, long double svy, long double swx, long double swy,
   long double avx, long double avy, long double awx, long double awy,


   long double v_heading, long double w_heading, long double th_heading,


   long double t2c_threshold, long double s2c_threshold,


   long double speed_ms_v, long double speed_ms_w,
   std::string laneID_v, std::string laneID_w
) {
  long double alpha_heading_diff = std::abs(v_heading - w_heading);

  std::cout << "!!!!!!!!!!!!!!diff_heading: " << alpha_heading_diff << std::endl;
  std::cout << "Vheading: " << v_heading << std::endl;
  std::cout << "Wheading: " << w_heading << std::endl;
//   bool is_rear_end_collision = (alpha_heading_diff <= 15 && alpha_heading_diff >= 345) || (alpha_heading_diff >= 165 && alpha_heading_diff <= 195);
    // bool is_rear_end_collision = (alpha_heading_diff <= 7.5 || alpha_heading_diff >= 352.5);
    bool is_rear_end_collision = (alpha_heading_diff <= 15);
  std::cout << "is_rear_end_collision: " << is_rear_end_collision << std::endl;
//   bool is_cross_road_collision = ((15 < alpha_heading_diff) && (alpha_heading_diff <= 165)) || ((alpha_heading_diff >= 195) && (alpha_heading_diff < 345));
bool is_cross_road_collision = (((7.5 < alpha_heading_diff) && (alpha_heading_diff <= 172.5))) || ((alpha_heading_diff >= 187.5) && (alpha_heading_diff < 352.5));
// bool is_cross_road_collision = ( (187.5<= alpha_heading_diff < 352.5));
// bool is_cross_road_collision = ((7.5 < alpha_heading_diff <= 172.5)) ;
  std::cout << "HEADINGTHRESHOLD: " << HEADING_THRESHOLD << std::endl;
  std::cout << "HEADINGTHRESHOLD_CROSS: " << HEADING_THRESHOLD_CROSS << std::endl;
  std::cout << "alpha_heading_diff: " << alpha_heading_diff << std::endl;
  std::cout << "is_cross_road_collision: " << is_cross_road_collision << std::endl;


//   long double t2c = time_to_collision(pvx, pvy, pwx, pwy, svx, svy, swx, swy, avx, avy, awx, awy);
//   long double s2c = space_to_collision(pvx, pvy, pwx, pwy, svx, svy, swx, swy, avx, avy, awx, awy, t2c);
//   std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXX t2c " << t2c << " s2c " << s2c << std::endl;


//&& (laneID_v == laneID_w)
  if (is_rear_end_collision) {
       if (is_in_range(pvx, pvy, pwx, pwy, svx, svy, swx, swy, t2c_threshold)) {
          
           long double t2c = time_to_collision(pvx, pvy, pwx, pwy, svx, svy, swx, swy, avx, avy, awx, awy);
           std::cout << "[TEST] Time to collision is " << t2c << " us, and th is " << t2c_threshold << " us." << std::endl;
           if (t2c < 0 || t2c > t2c_threshold) {
            // send_message(static_cast<uint32_t>(id_vehicle_v), static_cast<uint32_t>(id_vehicle_w), "rear-end");
               std::cout << "[TEST] Time to collision between vehicle " << id_vehicle_v << " and vehicle " << id_vehicle_w
               << " is " << t2c << " us. No collision risk detected." << std::endl;
               return false; // No collision risk detected
           }


           long double s2c = space_to_collision(pvx, pvy, pwx, pwy, svx, svy, swx, swy, avx, avy, awx, awy, t2c);
           std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!![TEST] Space to collision is " << s2c << " m, and th is " << s2c_threshold  << " m." << std::endl;
           if (s2c <= s2c_threshold) {
               std::cout << "[TEST] Rear end collision between vehicle " << id_vehicle_v << " and vehicle " << id_vehicle_w << std::endl;
               send_message(static_cast<uint32_t>(id_vehicle_v), static_cast<uint32_t>(id_vehicle_w), "cross-road");
               return true;
           }
       } else {
           std::cout << "[TEST] Vehicles are not in range for collision detection." << std::endl;
       }       


   } else   {

       if (is_in_range(pvx, pvy, pwx, pwy, svx, svy, swx, swy, t2c_threshold)) {
           // long double ego_heading_rad = v_heading * DEG_TO_RAD;
           // long double ego_dir_x = std::cos(ego_heading_rad);
           // long double ego_dir_y = std::sin(ego_heading_rad);
           // long double ego_lateral_dir_x = -ego_dir_y;
           // long double ego_lateral_dir_y = ego_dir_x;


           // long double rel_pos_x_m = pwx - pvx;
           // long double rel_pos_y_m = pwy - pvy;


           // long double lateral_distance_m = std::abs(rel_pos_x_m * ego_lateral_dir_x + rel_pos_y_m * ego_lateral_dir_y);
           // long double longitudinal_distance_m = rel_pos_x_m * ego_dir_x + rel_pos_y_m * ego_dir_y;


           // const long double MAX_LATERAL_DEVIATION_M = 1.0;


           // std::cout << "[DETECT_COLLISION] Vehicle " << id_vehicle_w << " relative to " << id_vehicle_v
           //           << ": LongitudinalDist_m: " << longitudinal_distance_m
           //           << ", LateralDist_m: " << lateral_distance_m << std::endl;


           // if (lateral_distance_m > MAX_LATERAL_DEVIATION_M) {
           //     std::cout << "[DETECT_COLLISION] Vehicle " << id_vehicle_w << " is laterally offset (in another lane?). No collision risk considered." << std::endl;
           //     return false;
           // }



           long double t2c = time_to_collision(pvx, pvy, pwx, pwy, svx, svy, swx, swy, avx, avy, awx, awy);
           std::cout << "[TEST] Time to collision is " << t2c << " us, and th is " << t2c_threshold << " us." << std::endl;
           if (t2c < 0 || t2c > t2c_threshold) {
               std::cout << "[TEST] Time to collision between vehicle " << id_vehicle_v << " and vehicle " << id_vehicle_w
               << " is " << t2c << " us. No collision risk detected." << std::endl;
               return false; // No collision risk detected
           }


           long double s2c = space_to_collision(pvx, pvy, pwx, pwy, svx, svy, swx, swy, avx, avy, awx, awy, t2c);
           // std::cout << "HEEEEEEEEEEEEEJ pvx: " << pvx << " pvy: " << pvy << " pwx: " << pwx << " pwy: " << pwy
           // << " svx: " << svx << " svy: " << svy << " swx: " << swx << " swy: " << swy
           // << " avx: " << avx << " avy: " << avy << " awx: " << awx << " awy: " << awy
           // << " t2c: " << t2c << " s2c: " << s2c << std::endl;
           std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!![TEST] Space to collision is " << s2c << " m, and th is " << s2c_threshold  << " m." << std::endl;
           std::cout << "Speed vehicle v: " << speed_ms_v << std::endl;
           std::cout << "Speed vehicle w: " << speed_ms_w << std::endl;
           if (s2c <= s2c_threshold) {
               std::cout << "[TEST] Cross road collision risk detected between vehicle " << id_vehicle_v << " and vehicle " << id_vehicle_w << std::endl;
               send_message(static_cast<uint32_t>(id_vehicle_v), static_cast<uint32_t>(id_vehicle_w), "cross-road");
               return true;
           }
       } else {
           std::cout << "[TEST] Vehicles are not in range for collision detection." << std::endl;
       }
   }


   return false;
}


bool CollisionAvoidance::is_in_range(
   long double pvx, long double pvy, long double pwx, long double pwy,
   long double svx, long double svy, long double swx, long double swy,
   long double t2c_threshold
) {
   std::cout << "[*******************IS_IN_RANGE******************] " << std::endl;
  
   long double pvx_new = pvx + svx * t2c_threshold;
   long double pvy_new = pvy + svy * t2c_threshold;
   long double pwx_new = pwx + swx * t2c_threshold;
   long double pwy_new = pwy + swy * t2c_threshold;


   long double distance_v = std::sqrt(std::pow(pvx_new - pvx, 2) + std::pow(pvy_new - pvy, 2));
   long double distance_w = std::sqrt(std::pow(pwx_new - pwx, 2) + std::pow(pwy_new - pwy, 2));


   long double distance = std::max(distance_v, distance_w);
   long double d_max = std::sqrt(2*std::pow(distance,2));


   long double distance_v2w = std::sqrt(std::pow(pvx - pwx, 2) + std::pow(pvy - pwy, 2));


   std::cout << "Distance v2w: " << distance_v2w << std::endl;
   std::cout << "Distance d_max: " << d_max << std::endl;
   std::cout << "Value: " << (distance_v2w < d_max) << std::endl;


   return distance_v2w < d_max;
}


long double CollisionAvoidance::time_to_collision(
   long double pvx, long double pvy, long double pwx, long double pwy,
   long double svx, long double svy, long double swx, long double swy,
   long double avx, long double avy, long double awx, long double awy
) {
   std::cout << "[*******************time_to_collision******************] " << std::endl;


   // Coefficients for the quadratic equation
//    long double a = 0.5 * (avx - awx) * (avx - awx) + 0.5 * (avy - awy) * (avy - awy);
//    long double b = (svx - swx) * (avx - awx) + (svy - swy) * (avy - awy);
//    long double c = (pvx - pwx) * (svx - swx) + (pvy - pwy) * (svy - swy);

long double a = 0.5 * (avx - awx)  + 0.5 * (avy - awy);
long double b = (svx - swx) + (svy - swy);
long double c = (pvx - pwx) + (pvy - pwy) ;


 // always log them
 std::cout << "coefficients: a="<< a
 << " b="<< b
 << " c="<< c << std::endl;


   // Handle degenerate (constant‐accel→zero) case
   // if (std::fabs(a) < 1e-8) {
   //     long double dvx = svx - swx;
   //     long double dvy = svy - swy;
   //     long double dv2 = dvx*dvx + dvy*dvy;
   //     if (dv2 > 1e-8) {
   //         // time of closest approach / collision for constant velocities
   //         long double tca = -((pvx - pwx)*dvx + (pvy - pwy)*dvy) / dv2;
   //         std::cout << "[constant‐vel] tca=" << tca << std::endl;
   //         return tca >= 0 ? tca : -1.0;
   //     }
   //     // no relative motion
   //     std::cout << "[no relative motion]" << std::endl;
   //     return -1.0;
   // }
  


   // Quadratic equation: at^2 + bt + c = 0
   long double discriminant = (b * b) - (4 * a * c);
   std::cout << "discriminant=" << discriminant << std::endl;
   if (discriminant < 0) {
       std::cout << "[no real roots] disc<0\n";
       return -1; // No real roots, no collision
   }
 
   long double t1 = (-b + std::sqrt(discriminant)) / (2 * a);
   long double t2 = (-b - std::sqrt(discriminant)) / (2 * a);


   std::cout << "t1: " << t1 << ", t2: " << t2 << std::endl;
   std::cout << "a: " << a << ", b: " << b << ", c: " << c << discriminant << std::endl;




   // Return the smallest non-negative root
   if (t1 >= 0 && t2 >= 0) {
       return std::min(t1, t2); // Both roots are non-negative, return the smaller one       
   } else if (t1 >= 0 && t2 < 0) {   
       return t1; // Only t1 is non-negative
   } else if (t2 >= 0 && t1 < 0) {       
       return t2; // Only t2 is non-negative
   } else {
       std::cout << "[both roots negative]\n";
       return -1; // Both roots are negative, no collision
   }
}


long double CollisionAvoidance::space_to_collision(
   long double pvx, long double pvy, long double pwx, long double pwy,
   long double svx, long double svy, long double swx, long double swy,
   long double avx, long double avy, long double awx, long double awy,
   long double t2c
) {
   std::cout << "[*******************space_to_collision******************] " << std::endl;
   long double pvx_new = pvx + (svx * t2c) + (0.5 * avx * std::pow (t2c,2));
   long double pvy_new = pvy + (svy * t2c) + (0.5 * avy * std::pow (t2c,2));
   long double pwx_new = pwx + (swx * t2c) + (0.5 * awx * std::pow (t2c,2));
   long double pwy_new = pwy + (swy * t2c) + (0.5 * awy * std::pow (t2c,2));
   std::cout << "pvx_new: " <<pvx_new << "pvy_new: "<<pvy_new<<"pwx_new: "<<pwx_new<<"pwy_new: " <<pwy_new<< std::endl;


   long double s2c = std::sqrt(std::pow((pvx_new - pwx_new),2) + std::pow((pvy_new - pwy_new),2));
    std::cout << "S2C: "<<s2c<< std::endl;
   return s2c;
}


void CollisionAvoidance::send_message(uint32_t id_vehicle_v, uint32_t id_vehicle_w, const std::string& collision_risk_type) {
   // Create a simple string message
   std::stringstream ss;
   ss << collision_risk_type << " " << id_vehicle_v << " " << id_vehicle_w;
   std::string message = ss.str();


   std::cout << "[CollisionAvoidance] Enqueue message: " << message << "\n";
   producer_ -> sendMessage(message);
}


void CollisionAvoidance::send_message(uint32_t recovered_vehicle_id, double speed_ms) {
   std::stringstream ss;
   ss << "recovered " << recovered_vehicle_id << " " << speed_ms;
   std::string message = ss.str();


   std::cout << "[CollisionAvoidance] Enqueue message: " << message << "\n";
   producer_->sendMessage(message);
}


void CollisionAvoidance::receive_message(ldmmap::LDMMap& ldmmap, const std::string& message) {
   std::stringstream ss(message);
   ldmmap::vehicleData_t veh;
   std::string id_token;
   double gn_sec_double;
   double tu_us_double;
 
   ss >> id_token;


   //error handling for id_token
   if (id_token.rfind("veh", 0) == 0) {
       try {
           veh.stationID = static_cast<uint32_t>(std::stoul(id_token.substr(3)));
       } catch (const std::invalid_argument& ia) {
           std::cerr << "[ERROR] Invalid stationID in message: " << id_token << std::endl;
           return;
       } catch (const std::out_of_range& oor) {
           std::cerr << "[ERROR] stationID out of range in message: " << id_token << std::endl;
           return;
       }
   } else {
       std::cerr << "[ERROR] Invalid vehicle ID format in message: " << id_token << std::endl;
       return;
   }


   if (!(ss >> veh.lat
            >> veh.lon
            >> veh.heading
            >> veh.elevation
            >> veh.speed_ms
            >> veh.acceleration_ms
            >> veh.laneID
            >> gn_sec_double
            >> tu_us_double)) {
       std::cerr << "[ERROR] Failed to parse vehicle data message for ID " << veh.stationID << ". Message: " << message << std::endl;
       return;
   }


   veh.gnTimestamp = gn_sec_double;
   veh.timestamp_us = static_cast<uint64_t>(tu_us_double);


   // sätt globalt format
   std::cout << std::fixed << std::setprecision(8);


   std::cout << "Parsed vehicleData_t object:\n"
             << "  stationID: "   << veh.stationID
             << "  lat: "         << veh.lat
             << "  lon: "         << veh.lon
             << "  elevation: "   << veh.elevation
             << "  heading: "     << veh.heading
             << "  speed_ms: "    << veh.speed_ms
             << "  acceleration_ms: " << veh.acceleration_ms
             << "  laneID: "       << veh.laneID
             << "  gnTimestamp: " << veh.gnTimestamp
             << "  timestamp_us: "<< veh.timestamp_us
             << std::endl;


   ldmmap::LDMMap::returnedVehicleData_t tmp;
   if (ldmmap.lookup(veh.stationID, tmp) != ldmmap::LDMMap::LDMMAP_OK) {
       ldmmap.insert(veh);
       std::cout << "[INFO] Added vehicle " << veh.stationID << " to LDMMap\n";
   }
   //kanske använda om inte funkar
   // // Uppdatera LDMMap
   // if (ldmmap.update(veh.stationID, veh) != ldmmap::LDMMap::LDMMAP_OK) {
   //     std::cerr << "[ERROR] Failed to update LDMMap for stationID: " << veh.stationID << std::endl;
   // }


   handle_vehicle_data(ldmmap, veh.stationID);


}
