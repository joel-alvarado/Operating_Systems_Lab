# Running IMUDriver with Supervisor
To run IMUDriver with supervisor, first configure CMake and run `make` inside the build folder.

After, you may run supervisor in the following way:

- `./supervisor <command_to_execute>`, or `./supervisor ./imu_driver` in our case

## What does it do?
Supervisor will monitor the process status every 1.5 seconds by looking for it's name in the current running processes. If not found, will fork and execute a new instance of the command.

## IMUDriver
IMUDriver is a simulation of a motion sensor driver with data logging. `imu_driver` will log sensor telemetry data to `data.txt` and time ellapsed since it was first started. This can be run along `supervisor` to allow for near-100% uptime on your driver process.