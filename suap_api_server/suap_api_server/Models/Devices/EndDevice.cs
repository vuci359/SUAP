public class SensorService implements SensorRepository{
  //  TODO
	public List<Sensor> sensors = new ArrayList<>();
	
	@Override
	public boolean register(Sensor sensor) {
		if (sensors.stream().
				anyMatch(a -> a.getLocation().equals(sensor.getLocation())))
	        {
	            System.out.println("Failed to register given sensor, sensor is already registered!");
	            return false;
	        }
	        else {
	            sensors.add(sensor);
	            System.out.println("Sensor " + sensor.getLocation() + " successfully registered!");
	            return true;
	        }
	    }

	public SensorAddress searchNeighbour(String location) {
	        if (sensors.size() == 1)  {
	            return null;
	        }

	        Map<SensorAddress, Double> distances = new HashMap<>();
	        Sensor givenSensor = sensors.stream()
	                .filter(x -> x.getLocation().equals(location))
	                .findFirst()
	                .orElse(null);

	        for (Sensor sensor : sensors) {
	            if (!sensor.getLocation().equals(location)) {
	                SensorAddress sensorAddress = new SensorAddress(sensor.getIpAddress(), sensor.getPort());
	                double distance = calculateDistanceBetweenSensors(givenSensor, sensor);
	                distances.put(sensorAddress, distance);
	                System.out.println("Distance between " + givenSensor.getLocation() + " and " + sensor.getLocation() + " is " + distance);
	            }
	        }
	        double shortestDistance = Collections.min(distances.values());
	        SensorAddress neighbourUserAddress = SensorRepository.getKeysByValue(distances, shortestDistance);
	        System.out.println("Shortest distance is " + shortestDistance);

	        return neighbourUserAddress;
	    }

	public List<Sensor> getSensors() {
        return sensors;
    }
}
