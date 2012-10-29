#ifndef _UGEAR_CURRENT_HXX
#define _UGEAR_CURRENT_HXX


#include "globals.hxx"


void compute_derived_data( struct gps *gpspacket,
			   struct imu *imupacket,
			   struct airdata *airpacket,
			   struct filter *filterpacket,
			   struct actuator *actpacket,
			   struct pilot *pilotpacket,
			   struct apstatus *appacket,
			   struct health *healthpacket);

void update_props( struct gps *gpspacket,
		   struct imu *imupacket,
		   struct airdata *airpacket,
		   struct filter *filterpacket,
		   struct actuator *actpacket,
		   struct pilot *pilotpacket,
		   struct apstatus *appacket,
		   struct health *healthpacket);

#endif // _UGEAR_CURRENT_HXX