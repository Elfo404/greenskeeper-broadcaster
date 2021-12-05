# Greenskeeper Broadcaster

A simple arduino nano BLE project that broadcasts data over BLE advertisement packets about house plants inspired by [rbaron/b-parasite](https://github.com/rbaron/b-parasite).

## Advertisement data

Data is advertised every 30 minutes for 10 seconds.

### Advertisement Payload Data Structure

| Byte(s) | Data                  |
|---------|-----------------------|
| 0       | Counter               |
| 1,2     | Temperature (Celsius) |
| 3,4     | Humidity              |
| 5,6     | Soil Moisture         |

## Schematics

![schematics](https://raw.githubusercontent.com/Elfo404/greenskeeper-broadcaster/main/schematics.png)

## Issues

### Power consumption

Ideally everything should turn off between advertisements in order to save energy, allowing the sensor to be powered by any cheap low-capacity battery over a long period of time. This is just a prototype, for a better working solution check out [rbaron/b-parasite](https://github.com/rbaron/b-parasite).

### VS Code issues with c/cpp completions

Add `"__STATIC_INLINE"` to the `define` array in `.vscode/c_cpp_properties.json`
