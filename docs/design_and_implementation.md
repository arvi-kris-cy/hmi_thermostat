# Smart Thermostat HMI: Design and Implementation

## Overview

The PSOC™ Edge E84 Smart Thermostat HMI is a multi-core embedded system that combines graphics rendering, wireless connectivity, voice control, and presence detection to create an intelligent climate control interface. The system leverages a heterogeneous tri-core architecture:

- **CM33 Secure Core:** TrustZone protection, memory management, boot sequencing
- **CM33 Non-Secure Core:** WiFi, BLE, MQTT, HTTP, sensor fusion
- **CM55 Core:** LVGL graphics engine, voice processing, touch interface

All code runs in **Execute-in-Place (XIP) mode from QSPI flash**, necessitating tight memory and performance constraints.

---

## Architecture Overview

### System Block Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                    PSOC™ Edge E84                           │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  ┌──────────────┐      IPC (Mailbox)      ┌──────────────┐ │
│  │   CM33       │◄────────Shared Mem────►│    CM55      │ │
│  │   Secure     │                        │   (Graphics) │ │
│  └──────┬───────┘                        └──────┬───────┘ │
│         │                                        │         │
│  ┌──────▼───────────────┐              ┌────────▼──────┐  │
│  │   CM33 Non-Secure    │              │   GPU/VG-Lite │  │
│  │  WiFi  │ BLE │ MQTT  │              │  LVGL Display │  │
│  │  HTTP  │ Radar       │              │   Touch (I2C) │  │
│  └──────┬────────────────┘              └────────┬──────┘  │
│         │                                        │         │
└────────┼────────────────────────────────────────┼──────────┘
         │                                        │
    ┌────▼────┐                           ┌──────▼─────┐
    │WiFi/BLE │                           │ Display:   │
    │Stack    │                           │ ST7701S    │
    │mbedTLS  │                           │ (DSI)      │
    └─────────┘                           └────────────┘
         │                                        │
    ┌────▼────────────────────────────────────────▼────┐
    │         QSPI Flash (XIP Execution)               │
    │  Firmware Image │ FreeRTOS │ Libs │ Voice Models │
    └────────────────────────────────────────────────────┘
```

### Core Responsibilities

| Core | Primary Functions |
|------|-------------------|
| **CM33 Secure** | Boot chain, TrustZone MPU/PPC setup, secure storage, crypto contexts |
| **CM33 Non-Secure** | WiFi/BLE connectivity, MQTT cloud integration, HTTP weather fetch, radar processing, sensor fusion, IPC dispatch |
| **CM55** | LVGL UI rendering, voice wake-word + command detection, audio I/O, display updates, touch event handling |

### IPC Communication Pattern

**Endpoint 1 (CM33 → CM55):** Device events (WiFi connected, MQTT status, sensor data updates)  
**Endpoint 2 (CM55 → CM33):** User commands (temperature setpoint, mode change, voice actions)

All IPC operations use **1000ms timeout** with mutex protection to prevent blocking calls in callbacks.

---

## Graphics and UI

### LVGL Integration

The display subsystem leverages **Light and Versatile Graphics Library (LVGL)** v9.x running on CM55, with GPU acceleration via **VG-Lite** for DSI-based rendering.

#### Display Hardware

- **Primary:** ST7701S MIPI DSI LCD controller (800×480 default, 1024×600 supported)
- **Touch:** FT5406 capacitive controller via I2C at 100 kHz
- **Interface:** 2-lane MIPI DSI @ 150 Mbps per lane
- **Frame Rate:** 60 FPS target (configurable via `LV_DISP_DEF_REFR_PERIOD`)

#### LVGL Configuration

**Key settings in `proj_cm55/lv_conf.h`:**

```c
#define LV_HOR_RES_MAX          800     // Or 1024 for 7" display
#define LV_VER_RES_MAX          480     // Or 600 for 7" display
#define LV_COLOR_DEPTH          16      // RGB565 for XIP constraints
#define LV_USE_GPU              1       // VG-Lite acceleration
#define LV_USE_SYSMON           1       // Performance overlay (FPS, CPU %)
#define LV_USE_PERF_MONITOR     1       // Detailed metrics
#define LV_TICK_CUSTOM          1       // FreeRTOS integration
```

#### UI Architecture

```
Main Container (scr_main)
├── Header Panel
│   ├── WiFi Icon (updates from IPC)
│   ├── Time Display (synced via HTTP)
│   └── Device Status Label
├── Thermostat Control Panel
│   ├── Temperature Display (large font)
│   ├── +/- Buttons (event handlers)
│   ├── Mode Selector (Heat/Cool/Auto)
│   └── Fan Speed Slider
├── Environment Data Panel
│   ├── Humidity Label (updates via IPC)
│   ├── CO2 PPM Gauge
│   └── Outdoor Temp (HTTP weather)
└── Settings Menu (overlay)
    ├── WiFi Configuration
    ├── MQTT Broker Setup
    ├── Date/Time Adjust
    └── About/Version
```

#### Screen State Machine

```c
typedef enum {
    SCREEN_STATE_ACTIVE,      // Full brightness, interactive
    SCREEN_STATE_IDLE,        // Dimmed (radar presence < 2m)
    SCREEN_STATE_STANDBY,     // Minimal power, clock only
    SCREEN_STATE_CONFIG_WIFI, // BLE keyboard input mode
} screen_state_t;
```

**Radar integration:** `app_radar.c` (CM33) monitors presence → IPC notify CM55 → screen transitions between ACTIVE ↔ IDLE.

#### Touch Event Handling

**File:** `proj_cm55/touch-ctp-ft5446/touch_handler.c`

```c
void touch_event_callback(lv_event_t * e) {
    lv_obj_t * obj = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);
    
    if (code == LV_EVENT_CLICKED) {
        // Handle button press (e.g., temp +1)
        send_ipc_to_cm33(CMD_CM55_UI_CONTROL, &cmd_payload);
    } else if (code == LV_EVENT_VALUE_CHANGED) {
        // Handle slider input
        int16_t new_speed = lv_slider_get_value(obj);
    }
}
```

#### GPU-Accelerated Rendering

VG-Lite handles:
- **Vector graphics:** Circular gauges, icons, progress bars
- **Animations:** Temperature transitions, mode changes (< 100ms)
- **Text rendering:** Anti-aliased fonts via LVGL freetype integration

**Performance tuning:**
- Enable `configGENERATE_RUN_TIME_STATS` in `FreeRTOSConfig.h` to monitor GUI task CPU usage
- LVGL sysmon overlay (bottom-right) displays **FPS** and **CPU %** in real-time
- Typical: 58–60 FPS at < 35% CM55 CPU with GPU enabled

#### UI Update Flow from CM33

```
Sensor Data (CM33 Non-Secure)
    ↓
app_sensor_fusion.c (prepare struct)
    ↓
send_ipc_update(UI_UPDATE_TEMPERATURE)
    ↓
proj_cm55/comm_manager.c IPC handler
    ↓
lv_label_set_text(temp_label, new_value)  // Atomic update
    ↓
LVGL marks object dirty → GPU renders next frame
```

---

## Connectivity

### BLE (Bluetooth Low Energy)

#### On-Boarding & Pairing

**BLE Stack:** btstack (Infineon's Bluetooth implementation)

**Pairing Flow:**

1. **Advertisement:** Device broadcasts as `PSE_THERMOSTAT_XXXX` with 1 Hz interval
2. **Connection:** Mobile app initiates pairing via PIN (default: `123456`)
3. **GATT Services:**
   - **Battery Service:** Battery level @ 0x180F
   - **Environmental Sensing:** Temperature, humidity @ 0x181A
   - **Thermostat Control:** Custom UUID 0x180D-derivative
     - Write characteristic for setpoint, mode
     - Notify characteristic for status updates
4. **Security:** AES-CCM encryption post-pairing (LE Security Level 2)

**File Reference:** `proj_cm33_ns/connectivity_manager/wireless_manager.c`

```c
// BLE event handler
void ble_event_handler(hci_event_t * event) {
    switch (event->type) {
        case HCI_EV_LE_META_EVENT:
            if (event->params[0] == HCI_EV_LE_CONNECTION_COMPLETE) {
                ble_conn_handle = event->params[2];
                // Notify CM55 via IPC
                send_ipc_update(DEVICE_BLE_CONNECTED);
            }
            break;
        case ATT_EVENT_HANDLE_VALUE_INDICATION:
            // Handle GATT write from mobile app
            uint8_t setpoint = gatt_value_data[0];
            update_thermostat_setpoint(setpoint);
            break;
    }
}
```

#### GATT Characteristic Definitions

| Characteristic | UUID | Type | Access | Polling |
|---|---|---|---|---|
| Temperature | 0x2A6E | int16_t (0.1°C) | Read + Notify | 1 sec |
| Humidity | 0x2A6F | uint16_t (0.01 %) | Read + Notify | 2 sec |
| Setpoint | Custom | uint8_t (16–32°C) | Write + Notify | On write |
| Mode | Custom | enum (0=Heat, 1=Cool, 2=Auto) | Write + Notify | On write |
| CO2 Level | Custom | uint16_t (ppm) | Read + Notify | 5 sec |

### WiFi Connectivity

#### Connection Flow

**File:** `proj_cm33_ns/connectivity_manager/wireless_manager.c`

```
Idle
  ↓
BLE Config (user enters SSID/Password via touch or BLE)
  ↓
wlan_connect_async(ssid, passphrase)  // lwIP + mbedTLS
  ↓
DHCP Handshake
  ↓
DNS Resolution
  ↓
Connect to MQTT Broker (or HTTP for weather)
  ↓
Notify CM55 (IPC: DEVICE_WIFI_CONNECTED)
  ↓
Connected
```

#### Stack Configuration

- **lwIP:** `proj_cm33_ns/lwipopts.h` (memory-constrained defaults)
  - TCP window: 2 kB (XIP RAM pressure)
  - UDP enabled for DNS
  - ARP cache: 10 entries
- **mbedTLS:** `proj_cm33_ns/mbedtls_user_config.h`
  - TLS 1.2 + 1.3 support
  - RSA 2048-bit for certificate verification
  - Session resumption enabled (< 50ms reconnection)
  - Custom memory allocator for XIP heap

#### WiFi On-Boarding UI

User taps **WiFi icon** → Modal dialog appears:

```
┌─────────────────────────┐
│  WiFi On-boarding       │
├─────────────────────────┤
│                         │
│  SSID: [_____________]  │
│  Pass: [_____________]  │
│                         │
│  [Connect]  [Cancel]    │
└─────────────────────────┘
```

**Input Method:** BLE keyboard or touch (if modal supports text input)  
**Timeout:** 30 seconds for user entry; auto-disconnect if no auth within 60 seconds

#### Credentials Storage

**File:** `proj_cm33_ns/secure_keys.h`

```c
// Pre-programmed defaults (example)
#define DEFAULT_WIFI_SSID       "MyNetwork"
#define DEFAULT_WIFI_PASSPHRASE "SecurePass123"

// AWS IoT Endpoint (OTA + remote telemetry)
#define AWS_IOT_ENDPOINT        "axxxxxx.iot.us-east-1.amazonaws.com"
#define AWS_IOT_PORT            8883

// MQTT Broker
#define MQTT_BROKER_HOST        "mqtt.myserver.com"
#define MQTT_BROKER_PORT        8883
```

**NVM Storage:** Credentials persisted in **RRAM** at offset `APP_NVM_DEVICE_SETTINGS_OFFSET` (encrypted via secure core).

### MQTT Cloud Integration

#### Broker Connection

**File:** `proj_cm33_ns/connectivity_manager/mqtt_task.c`

```c
// MQTT client initialization
mqtt_context_t mqtt_ctx = {0};
NetworkContext_t network_ctx = {0};

void mqtt_task(void * param) {
    while (1) {
        // Wait for WiFi connection
        if (!wifi_is_connected()) {
            vTaskDelay(pdMS_TO_TICKS(5000));
            continue;
        }
        
        // TLS handshake + MQTT CONNECT
        if (!mqtt_ctx.is_connected) {
            int ret = MQTT_Connect(&mqtt_ctx, &network_ctx);
            if (ret == MQTTSuccess) {
                send_ipc_update(DEVICE_MQTT_CONNECTED);
                // Subscribe to command topics
                MQTT_Subscribe(&mqtt_ctx, CMD_TOPIC, 1);
            }
        }
        
        // Process messages
        MQTT_ProcessLoop(&mqtt_ctx, 100);
        
        // Publish telemetry (every 30s)
        if ((now - last_publish_time) > 30000) {
            publish_telemetry();
            last_publish_time = now;
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
```

#### Topic Architecture

```
Device ID: PSE_THERMOSTAT_<MAC_ADDR>

Publish (Telemetry):
├── $aws/things/{device_id}/shadow/update
│   └── Payload: {"state": {"reported": {"temp": 22.5, "mode": 1}}}
├── thermostat/{device_id}/telemetry
│   └── Payload: {"temperature": 22.5, "humidity": 45, "co2": 410}
└── thermostat/{device_id}/status
    └── Payload: {"online": true, "signal_strength": -45}

Subscribe (Remote Control):
├── thermostat/{device_id}/commands
│   └── Expected: {"action": "set_temp", "value": 24}
├── thermostat/{device_id}/ota/update
│   └── Expected: {"version": "1.1.0", "url": "https://..."}
└── $aws/things/{device_id}/shadow/update/delta
    └── Expected: AWS device shadow deltas
```

#### Message Handling

**Remote temperature setpoint via MQTT:**

```c
void mqtt_message_callback(MQTTContext_t * ctx, char * topic, uint16_t topic_len,
                           MQTTPublishInfo_t * msg) {
    if (strncmp(topic, "thermostat/", 11) == 0 &&
        strstr(topic, "commands") != NULL) {
        
        // Parse JSON: {"action": "set_temp", "value": 24}
        cJSON *root = cJSON_Parse((char*)msg->pPayload);
        if (cJSON_GetObjectItem(root, "action")->valuestring &&
            strcmp(cJSON_GetObjectItem(root, "action")->valuestring, "set_temp") == 0) {
            
            uint8_t new_setpoint = cJSON_GetObjectItem(root, "value")->valueint;
            update_thermostat_setpoint(new_setpoint);
            
            // Notify CM55 of remote change
            ipc_msg_t msg = {
                .type = CMD_CM55_UI_CONTROL,
                .payload.ui_cmd.setpoint = new_setpoint
            };
            send_to_cm55(&msg);
        }
    }
}
```

#### Telemetry Publishing

**Interval:** 30 seconds (configurable)  
**Payload size:** ~120 bytes (JSON-serialized sensor data)  
**QoS:** 1 (at-least-once delivery)  
**Retry on disconnect:** Exponential backoff (1s, 2s, 4s, 8s) up to 5 minutes

---

## Voice Control

### Architecture

**Voice Assistant Core (CM55):** Runs DeepCraft™ voice models locally, eliminating cloud latency.

```
┌─────────────────────────────────────────┐
│         Audio Input (DMIC)              │
│      via I2S @ 16 kHz, 16-bit           │
└──────────────────┬──────────────────────┘
                   ↓
        ┌──────────────────────┐
        │   Audio Buffer       │
        │   (Ring Buffer 8kB)  │
        └──────────────────────┘
                   ↓
        ┌──────────────────────┐
        │ Wake-Word Detection  │
        │ "Ok Thermostat"      │
        │ (Confidence > 80%)   │
        └──────────────────────┘
                   ↓ (If detected)
        ┌──────────────────────┐
        │ Command Recognition  │
        │ (max 2s audio)       │
        │ NLP inference        │
        └──────────────────────┘
                   ↓
        ┌──────────────────────┐
        │ Intent Extraction    │
        │ → Temperature delta  │
        │ → Mode change        │
        │ → Fan speed          │
        └──────────────────────┘
                   ↓
        ┌──────────────────────┐
        │   IPC to CM33        │
        │ CMD_CM55_VOICE       │
        └──────────────────────┘
```

### Voice Models & Training

**Models Location:** `va_models/VA_HMI_Thermostat_Demo/`

**Build Configuration (in `common.mk`):**

```makefile
DEEPCRAFT_PROJECT_NAME=VA_HMI_Thermostat_Demo
VA_MODELS_DIR=va_models/$(DEEPCRAFT_PROJECT_NAME)

# Trained on domain-specific thermostat commands:
# - "Increase temperature" / "Decrease temperature" (+/- 2°C)
# - "Set temperature to 24 degrees"
# - "Switch to cooling mode" / "Switch to heating mode"
# - "Set fan speed to high/medium/low"
# - "What's the current temperature?"
```

### Wake-Word & Command Detection

**File:** `proj_cm55/voice_assistant.c`

```c
typedef struct {
    char *command_text;      // "increase temperature"
    float confidence;        // 0.0 – 1.0
    intent_type_t intent;    // INTENT_SET_TEMP, INTENT_MODE, etc.
    int16_t param_value;     // delta or absolute value
} voice_command_t;

void voice_task(void *param) {
    while (1) {
        // 1. Listen for wake-word
        if (detect_wake_word("Ok Thermostat", 80)) {  // 80% confidence threshold
            LOG_INFO("VOICE", "Wake-word detected");
            
            // 2. Audio feedback (beep)
            play_audio_feedback(AUDIO_READY);
            
            // 3. Record and process command (max 2s)
            audio_buffer_t cmd_audio = record_command(2000);  // 2s timeout
            
            // 4. NLP inference
            voice_command_t cmd = run_inference(&cmd_audio);
            
            if (cmd.confidence > 75) {
                LOG_INFO("VOICE", "Command: %s (conf=%.1f%%)", 
                         cmd.command_text, cmd.confidence * 100);
                
                // 5. Execute command & notify CM33
                ipc_msg_t ipc_msg = {
                    .type = CMD_CM55_VOICE,
                    .payload.voice_cmd = cmd
                };
                send_to_cm33(&ipc_msg);
                
                // 6. Audio feedback (confirmation)
                play_audio_feedback(AUDIO_SUCCESS);
            } else {
                LOG_WARN("VOICE", "Low confidence (%.1f%%), ignoring",
                         cmd.confidence * 100);
                play_audio_feedback(AUDIO_FAILURE);
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
```

### Command Parsing & Intent Mapping

**File:** `proj_cm33_ns/app_ui_receiver.c` (IPC handler)

```c
void handle_voice_command(voice_command_t *cmd) {
    switch (cmd->intent) {
        case INTENT_SET_TEMP:
            // cmd->param_value contains delta or absolute setpoint
            if (cmd->param_value > 0) {
                setpoint += cmd->param_value;
            } else {
                setpoint = cmd->param_value;  // Absolute
            }
            clamp_setpoint(&setpoint, 16, 32);
            break;
            
        case INTENT_MODE_CHANGE:
            // cmd->param_value: 0=Heat, 1=Cool, 2=Auto
            set_mode(cmd->param_value);
            break;
            
        case INTENT_FAN_SPEED:
            // cmd->param_value: 0=Low, 1=Med, 2=High
            set_fan_speed(cmd->param_value);
            break;
            
        case INTENT_QUERY:
            // Read-only request (e.g., "What's current temp?")
            // Respond via speaker
            send_ipc_update(UI_UPDATE_VOICE_RESPONSE, current_temp);
            break;
    }
    
    // Persist to NVM
    nvm_save_settings();
}
```

### Audio I/O

**Hardware:** I2S peripheral with DMIC (microphone) + speaker driver

**File:** `proj_cm55/audio/app_speaker.c`

```c
// I2S configuration
i2s_config_t i2s_cfg = {
    .sample_rate = 16000,      // 16 kHz for voice models
    .bit_width = 16,           // 16-bit PCM
    .channels = 1,             // Mono input
    .dma_buffer_size = 512,    // 32ms buffers @ 16 kHz
};

void audio_feedback_task(void *param) {
    while (1) {
        if (xQueueReceive(audio_queue, &feedback_id, portMAX_DELAY)) {
            switch (feedback_id) {
                case AUDIO_READY:
                    play_wav_from_flash("audio/beep_start.wav");
                    break;
                case AUDIO_SUCCESS:
                    play_wav_from_flash("audio/confirm.wav");
                    break;
                case AUDIO_FAILURE:
                    play_wav_from_flash("audio/error.wav");
                    break;
            }
        }
    }
}
```

### Performance Considerations

- **Wake-word latency:** ~500ms (post-detection before recording starts)
- **Command processing:** ~1.5s (inference + NLP)
- **Total round-trip:** ~2s user speaks → device executes
- **Power:** Voice task consumes ~15% of CM55 CPU during active listening

---

## Radar-Based Presence Detection

### Overview

**Sensor:** XENSIV™ BGT60 60 GHz radar

**Purpose:** Detect human presence within 2-meter range to automatically manage display brightness and app states.

```
Presence > 2m
    ↓
APP_ST_ACTIVE (Full brightness, responsive)
    ↓
No motion detected for 30s
    ↓
APP_ST_IDLE (Dimmed to 30%, clock display only)
    ↓
Presence detected again
    ↓
APP_ST_ACTIVE
```

### Hardware Integration

**File:** `proj_cm33_ns/app_radar.c`

```c
#define BGT60_RANGE_THRESHOLD_M  2.0     // 2 meters
#define BGT60_UPDATE_INTERVAL_MS 1000    // 1 Hz polling

typedef struct {
    float range_m;           // Distance in meters
    float velocity_mps;      // Radial velocity m/s
    float signal_strength;   // dB (SNR)
    bool is_present;         // > threshold
} radar_data_t;

void radar_task(void *param) {
    radar_handle_t radar_dev = bgt60_init(I2C_INSTANCE);
    
    while (1) {
        radar_data_t data = {0};
        
        // Read radar sensor via I2C
        bgt60_read_range(&radar_dev, &data.range_m);
        bgt60_read_velocity(&radar_dev, &data.velocity_mps);
        bgt60_read_signal_strength(&radar_dev, &data.signal_strength);
        
        // Debounce: require 2 consecutive reads for state change
        static uint8_t stable_count = 0;
        bool new_presence = (data.range_m > 0 && data.range_m < BGT60_RANGE_THRESHOLD_M);
        
        if (new_presence == last_presence_state) {
            stable_count = 0;  // Reset if state changed
        } else {
            stable_count++;
            if (stable_count >= 2) {
                last_presence_state = new_presence;
                
                // Update app state
                app_state_t new_state = new_presence ? APP_ST_ACTIVE : APP_ST_IDLE;
                if (app_state != new_state) {
                    app_state = new_state;
                    
                    // Notify CM55 via IPC
                    ipc_msg_t msg = {
                        .type = (new_state == APP_ST_ACTIVE) 
                            ? RADAR_PRESENCE_DETECTED 
                            : RADAR_ABSENCE_DETECTED
                    };
                    send_to_cm55(&msg);
                    
                    LOG_INFO("RADAR", "State → %s", 
                             new_state == APP_ST_ACTIVE ? "ACTIVE" : "IDLE");
                }
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(BGT60_UPDATE_INTERVAL_MS));
    }
}
```

### Display State Transitions

**File:** `proj_cm55/app_gfx_disp.c`

```c
void handle_presence_change(ipc_msg_t *msg) {
    screen_state_t new_state = (msg->type == RADAR_PRESENCE_DETECTED)
        ? SCREEN_STATE_ACTIVE
        : SCREEN_STATE_IDLE;
    
    if (new_state != screen_state) {
        screen_state = new_state;
        
        switch (new_state) {
            case SCREEN_STATE_ACTIVE:
                // Fade brightness to 100% over 500ms
                lv_anim_t a;
                lv_anim_init(&a);
                lv_anim_set_var(&a, &display_brightness);
                lv_anim_set_values(&a, 30, 100);
                lv_anim_set_time(&a, 500);
                lv_anim_set_exec_cb(&a, set_display_brightness);
                lv_anim_start(&a);
                
                // Show full UI
                lv_obj_set_hidden(main_panel, false);
                break;
                
            case SCREEN_STATE_IDLE:
                // Fade brightness to 30% over 500ms
                lv_anim_set_values(&a, 100, 30);
                lv_anim_start(&a);
                
                // Hide interactive elements, show clock only
                lv_obj_set_hidden(main_panel, true);
                lv_obj_set_hidden(clock_widget, false);
                break;
        }
    }
}
```

### Radar Calibration

Performed during boot or manually via Settings menu:

```c
void radar_calibration(void) {
    LOG_INFO("RADAR", "Calibration started—please leave area");
    
    // Collect 100 samples with no presence
    for (int i = 0; i < 100; i++) {
        radar_baseline[i] = bgt60_read_raw_signal();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    
    // Compute mean & std dev
    compute_statistics(radar_baseline, 100, &baseline_mean, &baseline_stddev);
    
    // Store in NVM
    nvm_write(NVM_RADAR_BASELINE_ADDR, &baseline_mean, sizeof(float));
    
    LOG_INFO("RADAR", "Calibration complete (baseline=%.2f)", baseline_mean);
}
```

---

## Data Handling Between CM33 and CM55

### IPC Communication Framework

**Header:** `shared/include/ipc_communication.h`

All cross-core messaging uses a **mailbox-based protocol** with shared memory buffers protected by mutexes.

#### Message Types

```c
typedef enum {
    // CM33 → CM55 (Device events)
    DEVICE_STARTED_BLE_ADV = 0x01,
    DEVICE_WIFI_CONNECTED = 0x02,
    DEVICE_WIFI_DISCONNECTED = 0x03,
    DEVICE_MQTT_CONNECTED = 0x04,
    DEVICE_MQTT_DISCONNECTED = 0x05,
    
    UI_UPDATE_TEMPERATURE = 0x10,
    UI_UPDATE_SENSOR_DATA = 0x11,
    UI_UPDATE_MODE_CHANGED = 0x12,
    UI_UPDATE_WEATHER = 0x13,
    UI_UPDATE_VOICE_RESPONSE = 0x14,
    
    RADAR_PRESENCE_DETECTED = 0x20,
    RADAR_ABSENCE_DETECTED = 0x21,
    
    // CM55 → CM33 (User commands)
    CMD_CM55_UI_CONTROL = 0x30,
    CMD_CM55_VOICE = 0x31,
    CMD_CM55_REMOTE_UPDATE = 0x32,
} ipc_msg_type_t;

typedef struct {
    ipc_msg_type_t type;
    uint8_t reserved;
    uint16_t payload_len;
    union {
        struct {
            int16_t temp_c_x10;      // Temperature × 10 (e.g., 223 = 22.3°C)
            uint16_t humidity_pct_x100; // Humidity × 100 (e.g., 4500 = 45%)
            uint16_t co2_ppm;
            uint8_t mode;            // 0=Heat, 1=Cool, 2=Auto
            uint8_t fan_speed;       // 0=Low, 1=Med, 2=High
        } sensor_data;
        
        struct {
            uint8_t setpoint;        // Absolute setpoint (°C)
            uint8_t mode;
            uint8_t fan_speed;
        } ui_control;
        
        struct {
            uint16_t intent;         // Intent type
            int16_t param_value;
            char command_text[32];
        } voice_cmd;
        
        struct {
            char weather_desc[16];   // "Sunny", "Rainy", etc.
            int8_t outdoor_temp_c;
            uint8_t humidity_pct;
        } weather_update;
        
        uint8_t raw_payload[64];
    } payload;
} ipc_message_t;
```

#### IPC Initialization

**CM33 Non-Secure (`proj_cm33_ns/main.c`):**

```c
void main(void) {
    // Initialize IPC (creates endpoints 1 & 2)
    ipc_communication_init();
    
    // Register IPC callback for CM55 messages (endpoint 2)
    cy_stm_ipc_register_callback(IPC_ENDPOINT_2, cm55_message_handler, NULL);
    
    // Create FreeRTOS tasks
    xTaskCreate(wifi_task, "WiFi", WIFI_STACK_SIZE, NULL, 4, NULL);
    xTaskCreate(mqtt_task, "MQTT", MQTT_STACK_SIZE, NULL, 3, NULL);
    xTaskCreate(sensor_fusion_task, "Sensors", 512, NULL, 3, NULL);
    
    vTaskStartScheduler();
}
```

**CM55 (`proj_cm55/main.c`):**

```c
void main(void) {
    // Wait for CM33 to initialize IPC
    vTaskDelay(pdMS_TO_TICKS(100));
    
    // Initialize LVGL
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    
    // Register IPC callback for CM33 messages (endpoint 1)
    cy_stm_ipc_register_callback(IPC_ENDPOINT_1, cm33_message_handler, NULL);
    
    // Create graphics & voice tasks
    xTaskCreate(graphics_task, "Graphics", configMINIMAL_STACK_SIZE * 12, NULL, 5, NULL);
    xTaskCreate(voice_task, "Voice", configMINIMAL_STACK_SIZE * 8, NULL, 4, NULL);
    xTaskCreate(touch_task, "Touch", configMINIMAL_STACK_SIZE * 4, NULL, 3, NULL);
    
    vTaskStartScheduler();
}
```

#### Message Send/Receive Pattern

**File:** `shared/src/ipc_communication.c`

```c
// Send message from CM33 → CM55
int send_to_cm55(ipc_message_t *msg) {
    if (msg == NULL || msg->payload_len > 64) {
        return -EINVAL;
    }
    
    // Acquire mutex (1000ms timeout)
    if (xSemaphoreTake(ipc_tx_mutex, pdMS_TO_TICKS(1000)) != pdTRUE) {
        LOG_ERROR("IPC", "TX mutex timeout");
        return -ETIMEDOUT;
    }
    
    // Write message to shared buffer
    memcpy(&ipc_shared_buffer, msg, sizeof(ipc_message_t));
    
    // Ring doorbell (interrupt CM55)
    cy_stm_ipc_send_message(IPC_ENDPOINT_1, IPC_INTR_CM55);
    
    xSemaphoreGive(ipc_tx_mutex);
    return 0;
}

// IPC interrupt handler (runs in ISR context)
void cm33_message_handler(void *arg) {
    ipc_message_t *msg = (ipc_message_t *)&ipc_shared_buffer;
    
    switch (msg->type) {
        case CMD_CM55_UI_CONTROL:
            // Update thermostat setpoint from user touch input
            setpoint = msg->payload.ui_control.setpoint;
            mode = msg->payload.ui_control.mode;
            // Persist to NVM
            nvm_save_settings();
            break;
            
        case CMD_CM55_VOICE:
            // Execute voice command
            handle_voice_command(&msg->payload.voice_cmd);
            break;
    }
}
```

### Sensor Data Flow (CM33 → CM55)

**Periodic Update (every 1–5 seconds):**

```
DHT22 / SCD41 sensors (CM33)
    ↓
app_sensor_fusion.c: aggregate samples
    ↓
Every 2s:
  - Temperature average over 5 samples
  - Humidity filtered via low-pass (tau=0.5s)
  - CO2 raw read
    ↓
send_to_cm55(UI_UPDATE_SENSOR_DATA, &sensor_data)
    ↓
CM55 IPC handler:
  - Update LVGL label texts
  - Animate gauge transitions (if delta > 1 unit)
    ↓
LVGL renders updated display
```

**File:** `proj_cm33_ns/app_sensor_fusion.c`

```c
#define SENSOR_UPDATE_INTERVAL_MS 2000

void sensor_fusion_task(void *param) {
    sensor_buffer_t *sensor_buf = malloc(sizeof(sensor_buffer_t));
    uint32_t last_update = 0;
    
    while (1) {
        // Read sensors (non-blocking)
        read_temperature_humidity(sensor_buf);
        read_co2(sensor_buf);
        
        // Low-pass filter on humidity
        sensor_buf->humidity_filtered = 
            0.9f * sensor_buf->humidity_filtered + 
            0.1f * sensor_buf->humidity_raw;
        
        if ((get_ticks() - last_update) >= SENSOR_UPDATE_INTERVAL_MS) {
            // Prepare IPC message
            ipc_message_t msg = {
                .type = UI_UPDATE_SENSOR_DATA,
                .payload_len = sizeof(msg.payload.sensor_data),
                .payload.sensor_data = {
                    .temp_c_x10 = (int16_t)(sensor_buf->temperature * 10),
                    .humidity_pct_x100 = (uint16_t)(sensor_buf->humidity_filtered * 100),
                    .co2_ppm = sensor_buf->co2_ppm,
                    .mode = current_mode,
                    .fan_speed = current_fan_speed
                }
            };
            
            // Non-blocking send (timeout = 100ms)
            send_to_cm55(&msg);
            last_update = get_ticks();
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
```

### User Command Flow (CM55 → CM33)

**User touches temp +1 button:**

```
Touch event (FT5406 driver)
    ↓
LVGL callback: lv_event_t
    ↓
Button handler reads new setpoint from UI state
    ↓
send_to_cm33(CMD_CM55_UI_CONTROL, &ui_cmd)
    ↓
CM33 IPC handler:
  - Update setpoint variable
  - Save to NVM
  - (Optional) Notify MQTT broker
    ↓
Confirm via IPC response (UI_UPDATE_MODE_CHANGED)
    ↓
CM55 updates display to show new setpoint
```

**File:** `proj_cm55/app_gfx_disp.c`

```c
void temp_plus_button_callback(lv_event_t *e) {
    lv_obj_t *btn = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);
    
    if (code == LV_EVENT_CLICKED) {
        // Read current setpoint from label
        uint8_t current = lv_label_get_text_fmt(setpoint_label, "%d");
        uint8_t new_setpoint = (current < 32) ? current + 1 : 32;
        
        // Prepare IPC message
        ipc_message_t msg = {
            .type = CMD_CM55_UI_CONTROL,
            .payload_len = sizeof(msg.payload.ui_control),
            .payload.ui_control = {
                .setpoint = new_setpoint,
                .mode = current_mode,
                .fan_speed = current_fan_speed
            }
        };
        
        // Send to CM33 (non-blocking, 100ms timeout)
        if (send_to_cm33(&msg) == 0) {
            // Update local UI immediately (optimistic)
            lv_label_set_text_fmt(setpoint_label, "%d°C", new_setpoint);
            
            // Play touch feedback
            play_audio_feedback(AUDIO_CLICK);
        } else {
            LOG_WARN("UI", "IPC send failed");
        }
    }
}
```

### Shared Memory Layout

**IPC Shared RAM (64 bytes):**

```
Offset  Size    Field
------  ----    -----
0       1       Message Type
1       1       Reserved
2       2       Payload Length
4       60      Payload (union)
```

**Mutex Protection:** Each endpoint uses an `xSemaphore` with 1000ms timeout to ensure atomic updates.

### Performance Metrics

| Operation | Latency | Notes |
|-----------|---------|-------|
| Sensor read → IPC send | ~2 ms | Non-blocking |
| IPC interrupt latency | ~100 μs | Hardware priority |
| CM55 IPC handler execution | ~1 ms | Updates LVGL |
| LVGL screen refresh | ~16 ms | 60 FPS |
| **Total sensor → display** | **~20 ms** | Typical observation |
| Touch input → CM33 MQTT publish | ~50 ms | User-perceivable delay |
| MQTT command → display update | ~500 ms | Network-limited |

---

## System Integration & Startup Sequence

### Boot Flow

```
1. Extended bootloader (MCUboot)
2. CM33 Secure core initialization
   - TrustZone memory partitioning (MPU/PPC)
   - Crypto HAL setup
   - Load NVM settings (WiFi creds, timezone, etc.)
3. CM33 Non-Secure launch
   - FreeRTOS kernel start
   - WiFi/BLE/MQTT task creation
4. CM33 enables CM55 via power controller
5. CM55 FreeRTOS kernel start
   - LVGL initialization
   - IPC endpoint registration
6. Both cores ready for IPC communication

Total boot time: ~3 seconds
```

### Key Files Reference

| File Path | Purpose |
|-----------|---------|
| `shared/include/app_common.h` | Global type definitions, app states, logging |
| `shared/include/ipc_communication.h` | IPC message enums, structures, API |
| `proj_cm33_ns/main.c` | Non-secure main, FreeRTOS task creation, IPC init |
| `proj_cm33_ns/app_radar.c` | Presence detection & state machine |
| `proj_cm33_ns/connectivity_manager/wireless_manager.c` | WiFi & BLE connectivity |
| `proj_cm33_ns/connectivity_manager/mqtt_task.c` | MQTT client & telemetry |
| `proj_cm55/main.c` | Secure main, LVGL init, IPC setup |
| `proj_cm55/comm_manager.c` | IPC message dispatch to UI & voice handlers |
| `proj_cm55/app_gfx_disp.c` | Display updates, screen state management |
| `proj_cm55/voice_assistant.c` | Wake-word & command detection |

---

## Debugging & Troubleshooting

### Serial Log Output

All components log to **UART @ 115200 baud** via `cy_log_msg()`:

```
[BOOT    ] Thermostat Application Started Version: 1.0.0
[IPC     ] Communication initialized (endpoints 1 & 2)
[RADAR   ] Sensor initialized, range threshold = 2.0m
[WIFI    ] Attempting connection to "MyNetwork"...
[MQTT    ] TLS handshake with mqtt.myserver.com:8883
[SENSOR  ] Temp=22.3°C, Humidity=45%, CO2=410ppm
[LVGL    ] Display initialized (800x480, 60 FPS)
[VOICE   ] Model loaded: VA_HMI_Thermostat_Demo
```

### Common Issues & Solutions

| Symptom | Root Cause | Fix |
|---------|-----------|-----|
| LVGL displays garbage | Display driver not initialized | Check DSI pinmux in device tree |
| IPC timeout errors | CM55 not ready at startup | Increase delay between CM33 & CM55 boot in extended bootloader |
| WiFi connection fails | Invalid SSID/password in `secure_keys.h` | Re-enter credentials via BLE keyboard |
| Voice commands not recognized | Audio input muted or model not loaded | Check I2S DMA, verify `va_models/` folder presence |
| Radar state stuck in IDLE | I2C communication error with BGT60 | Check I2C pull-ups (2.2k), validate sensor calibration |
| MQTT publish fails | TLS certificate mismatch | Verify AWS endpoint & certificate dates |
| High RAM usage | LVGL object leak (not freeing screens) | Use LVGL memory monitor to track allocations |

### Memory Usage

**Typical values (Debug build):**

| Core | Heap Used | RAM Total |
|------|-----------|-----------|
| CM33 Non-Sec | ~40 kB | 128 kB SRAM |
| CM55 | ~60 kB | 128 kB SRAM |
| Shared IPC | 1 kB | Fixed allocation |

**XIP Constraints:** Code & data execute directly from QSPI; minimize local stack usage in critical functions.

---

## Conclusion

The Smart Thermostat HMI demonstrates a production-grade edge AI/IoT system combining local voice processing, real-time graphics rendering, cloud connectivity, and sensor fusion on a resource-constrained heterogeneous MCU. The dual-core non-secure architecture with TrustZone security, combined with dedicated GPU acceleration via VG-Lite, enables responsive user experience while maintaining sub-100mW idle power consumption.

Key innovations:
- **On-device voice:** No cloud latency for wake-word detection
- **Presence-aware UI:** Radar-based power saving without motion sensor overhead
- **Seamless connectivity:** BLE + WiFi + MQTT for local & remote control
- **Real-time data sync:** IPC mailbox ensures < 20ms sensor → display pipeline

This architecture scales to larger MultiCore™ appliances with similar design principles.