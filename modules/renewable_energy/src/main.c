/* renewable_energy: Renewable energy modeling (v1.0)
 * Solar PV, wind turbines, energy storage, forecasting, grid integration
 */
#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_ARRAYS     16
#define MAX_TURBINES   16
#define MAX_STORAGE    8
#define MAX_HOURLY     24
#define MAX_FORECAST   48

#define PV_MONO  0x01
#define PV_POLY  0x02
#define PV_THINF 0x03
#define PV_BIFAC 0x04

#define WIND_HAWT 0x01
#define WIND_VAWT 0x02
#define WIND_OFFSHORE 0x03

#define BATT_LION  0x01
#define BATT_FLOW  0x02
#define BATT_SODIUM 0x03
#define BATT_PUMPED 0x04

#define MPPT_PO    0x01
#define MPPT_IC    0x02
#define MPPT_FUZZY 0x03

#define FC_ELECTROLYZER 0x01
#define FC_FUELCELL 0x02

typedef struct {
    int    type;
    double area;
    double efficiency;
    double P_max;
    double P_mp;
    double V_mp;
    double I_sc;
    double V_oc;
    double temp_coeff;
    double T_cell;
    double irradiance;
    double tilt;
    double azimuth;
    double power_output;
    int    active;
} pv_array_t;

typedef struct {
    int    type;
    double rotor_diameter;
    double hub_height;
    double rated_power;
    double cut_in;
    double rated_speed;
    double cut_out;
    double wind_speed;
    double Cp;
    double power_output;
    int    active;
} wind_turbine_t;

typedef struct {
    int    type;
    double capacity;
    double soc;
    double soc_max;
    double soc_min;
    double charge_eff;
    double discharge_eff;
    double power_in;
    double power_out;
    double cycles;
    double degradation;
    int    active;
} storage_t;

typedef struct {
    double speed[MAX_FORECAST];
    double direction[MAX_FORECAST];
    double temperature[MAX_FORECAST];
    double irradiance[MAX_FORECAST];
    double pressure[MAX_FORECAST];
    int    hours;
    double avg_speed;
    double max_speed;
    double avg_irradiance;
} weather_forecast_t;

typedef struct {
    double power_curve[10];
    double wind_speeds[10];
    int    curve_points;
    double A, k;
    double avg_power;
    double capacity_factor;
} wind_model_t;

typedef struct {
    double pressure;
    double temperature;
    double voltage;
    double current;
    double H2_flow;
    double efficiency;
    int    type;
    int    active;
} hydrogen_t;

typedef struct {
    double P_actual;
    double P_forecast;
    double error;
    double MAE;
    double RMSE;
    int    n_points;
    double confidence;
} forecast_accuracy_t;

typedef struct {
    int    pv_count;
    int    wind_count;
    int    storage_count;
    int    h2_count;
    double total_pv;
    double total_wind;
    double total_storage;
    double total_load;
    double total_curtailment;
    double LCOE;
    double capacity_factor;
    double renewable_fraction;
    int    grid_connected;
} re_state_t;

static pv_array_t pvs[MAX_ARRAYS];
static wind_turbine_t winds[MAX_TURBINES];
static storage_t storage[MAX_STORAGE];
static weather_forecast_t weather;
static wind_model_t wind_model;
static hydrogen_t hydrogen[4];
static forecast_accuracy_t fc;
static re_state_t re;

static int initialized = 0;

static void print_str(const char* s) { host_print(s); }
static void print_int(int val) {
    char buf[32]; int i = 0;
    if (val < 0) { buf[i++] = '-'; val = -val; }
    if (val == 0) { buf[i++] = '0'; }
    else { int s = i; while (val > 0) { buf[i++] = '0' + (val % 10); val /= 10; }
           int e = i - 1; while (s < e) { char t = buf[s]; buf[s] = buf[e]; buf[e] = t; s++; e--; } }
    buf[i] = '\0'; host_print(buf);
}
static double dabs(double x) { return x < 0 ? -x : x; }
static double dsqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x, p = x / g;
    for (int i = 0; i < 30; i++) { g = (g + p) * 0.5; p = x / g; }
    return g;
}
static double dcos(double x) {
    double sum = 1.0, term = 1.0, x2 = x * x;
    for (int i = 1; i < 15; i++) { term *= -x2 / ((2*i-1)*(2*i)); sum += term; }
    return sum;
}
static double dexp(double x) {
    if (x > 20.0) return 5e8;
    if (x < -20.0) return 0.0;
    double sum = 1.0, term = 1.0;
    for (int i = 1; i < 30; i++) { term *= x / i; sum += term; }
    return sum;
}
static double dlog(double x) {
    if (x <= 0.0) return -100.0;
    double y = (x - 1.0) / (x + 1.0);
    double y2 = y * y;
    double sum = 0.0, term = y;
    for (int i = 0; i < 20; i++) {
        sum += term / (2 * i + 1);
        term *= y2;
    }
    return 2.0 * sum;
}

int re_init(void) {
    if (initialized) return -1;
    re.pv_count = 0; re.wind_count = 0; re.storage_count = 0; re.h2_count = 0;
    re.total_pv = 0.0; re.total_wind = 0.0; re.total_storage = 0.0;
    re.total_load = 0.0; re.total_curtailment = 0.0;
    re.LCOE = 0.0; re.capacity_factor = 0.0; re.renewable_fraction = 0.0;
    re.grid_connected = 1;
    for (int i = 0; i < MAX_ARRAYS; i++) pvs[i].active = 0;
    for (int i = 0; i < MAX_TURBINES; i++) winds[i].active = 0;
    for (int i = 0; i < MAX_STORAGE; i++) storage[i].active = 0;
    weather.hours = 48;
    for (int i = 0; i < MAX_FORECAST; i++) {
        double h = (double)i;
        double hour_angle = (h / 24.0) * 2.0 * 3.14159;
        weather.irradiance[i] = 800.0 * (dcos(hour_angle) > 0 ? dcos(hour_angle) : 0.0);
        weather.speed[i] = 8.0 + 4.0 * dcos(hour_angle * 0.5);
        weather.temperature[i] = 25.0 + 10.0 * dcos(hour_angle);
        weather.direction[i] = 180.0 + 30.0 * dcos(hour_angle * 0.3);
        weather.pressure[i] = 1013.0 + 10.0 * dcos(hour_angle * 0.2);
    }
    weather.avg_speed = 0.0; weather.max_speed = 0.0; weather.avg_irradiance = 0.0;
    for (int i = 0; i < weather.hours; i++) {
        weather.avg_speed += weather.speed[i];
        weather.avg_irradiance += weather.irradiance[i];
        if (weather.speed[i] > weather.max_speed) weather.max_speed = weather.speed[i];
    }
    weather.avg_speed /= weather.hours;
    weather.avg_irradiance /= weather.hours;
    wind_model.curve_points = 0;
    wind_model.avg_power = 0.0; wind_model.capacity_factor = 0.0;
    wind_model.A = 0.0; wind_model.k = 2.0;
    fc.n_points = 0; fc.error = 0.0; fc.MAE = 0.0; fc.RMSE = 0.0;
    initialized = 1;
    print_str("[RE] Renewable energy module initialized\n");
    return 0;
}

int re_add_pv(int type, double area, double efficiency, double tilt, double azimuth) {
    if (re.pv_count >= MAX_ARRAYS) return -1;
    pv_array_t* p = &pvs[re.pv_count];
    p->type = type; p->area = area; p->efficiency = efficiency;
    p->tilt = tilt; p->azimuth = azimuth;
    p->temp_coeff = -0.004;
    p->T_cell = 25.0;
    p->irradiance = 1000.0;
    double G_STC = 1000.0;
    p->P_max = area * efficiency * G_STC;
    p->P_mp = p->P_max * 0.85;
    p->V_mp = 30.0 + area * 0.1;
    p->I_sc = area * 0.03;
    p->V_oc = p->V_mp * 1.2;
    p->power_output = 0.0;
    p->active = 1;
    return re.pv_count++;
}

int re_add_wind(int type, double diameter, double hub_height, double rated_power) {
    if (re.wind_count >= MAX_TURBINES) return -1;
    wind_turbine_t* w = &winds[re.wind_count];
    w->type = type; w->rotor_diameter = diameter;
    w->hub_height = hub_height; w->rated_power = rated_power;
    w->cut_in = 3.0; w->rated_speed = 12.0; w->cut_out = 25.0;
    w->wind_speed = 0.0;
    w->Cp = 0.45;
    w->power_output = 0.0;
    w->active = 1;
    return re.wind_count++;
}

int re_add_storage(int type, double capacity, double soc_init) {
    if (re.storage_count >= MAX_STORAGE) return -1;
    storage_t* s = &storage[re.storage_count];
    s->type = type; s->capacity = capacity;
    s->soc = soc_init; s->soc_max = 95.0; s->soc_min = 5.0;
    s->charge_eff = 0.92; s->discharge_eff = 0.92;
    s->power_in = 0.0; s->power_out = 0.0;
    s->cycles = 0.0; s->degradation = 0.0;
    s->active = 1;
    return re.storage_count++;
}

void re_pv_model(int array_id, int mppt_method) {
    if (array_id >= re.pv_count) return;
    pv_array_t* p = &pvs[array_id];
    double G = p->irradiance;
    double T = p->T_cell;
    double P_ref = p->P_max;
    double gamma = p->temp_coeff;
    double G_STC = 1000.0;
    double T_STC = 25.0;
    double P_mppt = P_ref * (G / G_STC) * (1.0 + gamma * (T - T_STC));
    if (mppt_method == MPPT_PO) {
        double dP = P_mppt * 0.001;
        double dV = 0.1;
        double P_prev = P_mppt - dP;
        if (dP > 0) p->V_mp += dV;
        else p->V_mp -= dV;
    } else if (mppt_method == MPPT_IC) {
        double I = p->I_sc * G / G_STC;
        double V = p->V_mp;
        double dI_dV = -I / (V + 1e-10);
        double I_mp = I - V * dI_dV;
        (void)I_mp;
    }
    p->power_output = P_mppt * p->efficiency;
    re.total_pv += p->power_output;
    print_str("[RE] PV array "); print_int(array_id);
    print_str(" G="); print_int((int)G); print_str("W/m2");
    print_str(" T="); print_int((int)T); print_str("C");
    print_str(" P="); print_int((int)(p->power_output * 1000)); print_str("W\n");
}

void re_wind_model(int turbine_id) {
    if (turbine_id >= re.wind_count) return;
    wind_turbine_t* w = &winds[turbine_id];
    double v = w->wind_speed;
    double rho = 1.225;
    double A = 3.14159 * (w->rotor_diameter * 0.5) * (w->rotor_diameter * 0.5);
    if (v < w->cut_in || v > w->cut_out) {
        w->power_output = 0.0;
    } else if (v < w->rated_speed) {
        w->power_output = 0.5 * rho * A * v * v * v * w->Cp;
        if (w->power_output > w->rated_power) w->power_output = w->rated_power;
    } else {
        w->power_output = w->rated_power;
    }
    re.total_wind += w->power_output;
    print_str("[RE] Wind turbine "); print_int(turbine_id);
    print_str(" v="); print_int((int)(v * 10)); print_str("0dms");
    print_str(" P="); print_int((int)(w->power_output * 1000)); print_str("W");
    print_str(" Cp="); print_int((int)(w->Cp * 100)); print_str("%\n");
}

void re_weibull_distribution(double A, double k) {
    wind_model.A = A;
    wind_model.k = k;
    wind_model.curve_points = 10;
    double total_energy = 0.0;
    for (int i = 0; i < 10; i++) {
        double v = 3.0 + i * 2.0;
        wind_model.wind_speeds[i] = v;
        double f_v = (k / A) * dexp(-dlog(v / A) * k + k - 1.0);
        double P_v = 0.5 * 1.225 * 3.14159 * 40.0 * 40.0 * v * v * v * 0.45;
        if (P_v > 2000000.0) P_v = 2000000.0;
        wind_model.power_curve[i] = P_v * f_v;
        total_energy += wind_model.power_curve[i];
    }
    wind_model.avg_power = total_energy / 10.0;
    wind_model.capacity_factor = wind_model.avg_power / 2000000.0;
    print_str("[RE] Weibull A="); print_int((int)A);
    print_str(" k="); print_int((int)(k * 100)); print_str("\n");
    print_str("  Avg power="); print_int((int)(wind_model.avg_power / 1000));
    print_str("kW CF="); print_int((int)(wind_model.capacity_factor * 100));
    print_str("%\n");
}

void re_storage_dispatch(int storage_id, double power_target) {
    if (storage_id >= re.storage_count) return;
    storage_t* s = &storage[storage_id];
    if (power_target > 0.0) {
        double P_in = power_target;
        if (P_in > s->capacity * 0.2) P_in = s->capacity * 0.2;
        double E_stored = P_in * s->charge_eff;
        double new_soc = s->soc + E_stored / (s->capacity + 1e-10) * 100.0;
        if (new_soc > s->soc_max) { P_in = 0.0; new_soc = s->soc; }
        s->power_in = P_in;
        s->power_out = 0.0;
        s->soc = new_soc;
    } else {
        double P_out = -power_target;
        if (P_out > s->capacity * 0.2) P_out = s->capacity * 0.2;
        double E_drawn = P_out / s->discharge_eff;
        double new_soc = s->soc - E_drawn / (s->capacity + 1e-10) * 100.0;
        if (new_soc < s->soc_min) { P_out = 0.0; new_soc = s->soc; }
        s->power_in = 0.0;
        s->power_out = P_out;
        s->soc = new_soc;
        s->cycles += 0.01;
        s->degradation += 0.001;
    }
    print_str("[RE] Storage "); print_int(storage_id);
    print_str(" SOC="); print_int((int)s->soc); print_str("%");
    print_str(" P_in="); print_int((int)(s->power_in * 1000)); print_str("W");
    print_str(" P_out="); print_int((int)(s->power_out * 1000)); print_str("W\n");
}

void re_forecast_accuracy(int n_hours) {
    fc.n_points = n_hours;
    fc.error = 0.0; fc.MAE = 0.0; fc.RMSE = 0.0;
    double sum_err2 = 0.0;
    for (int h = 0; h < n_hours && h < weather.hours; h++) {
        double actual = re.total_pv + re.total_wind;
        double forecast = actual * (0.9 + 0.2 * dcos(h * 0.3));
        double err = forecast - actual;
        fc.error += err;
        fc.MAE += dabs(err);
        sum_err2 += err * err;
    }
    fc.error /= n_hours;
    fc.MAE /= n_hours;
    fc.RMSE = dsqrt(sum_err2 / n_hours);
    fc.confidence = 95.0 - fc.RMSE / (re.total_pv + re.total_wind + 1e-10) * 100.0;
    if (fc.confidence < 0.0) fc.confidence = 0.0;
    print_str("[RE] Forecast accuracy ("); print_int(n_hours); print_str("h):\n");
    print_str("  MAE="); print_int((int)fc.MAE); print_str("kW");
    print_str(" RMSE="); print_int((int)fc.RMSE); print_str("kW");
    print_str(" confidence="); print_int((int)fc.confidence); print_str("%\n");
}

void re_compute_lcoe(double capex, double opex, double lifetime, double discount_rate) {
    double annual_energy = (re.total_pv + re.total_wind) * 8760.0 * re.capacity_factor;
    double CRF = discount_rate * dexp(discount_rate * lifetime) / (dexp(discount_rate * lifetime) - 1.0);
    double annual_cost = capex * CRF + opex;
    re.LCOE = annual_cost / (annual_energy + 1e-10);
    print_str("[RE] LCOE calculation:\n");
    print_str("  Capex=$"); print_int((int)capex);
    print_str(" Opex=$"); print_int((int)opex); print_str("/yr\n");
    print_str("  Annual energy="); print_int((int)(annual_energy / 1000));
    print_str("MWh LCOE=$"); print_int((int)(re.LCOE * 100)); print_str("/MWh\n");
}

void re_print_state(void) {
    print_str("[RE] PV="); print_int(re.pv_count);
    print_str(" Wind="); print_int(re.wind_count);
    print_str(" Storage="); print_int(re.storage_count);
    print_str(" H2="); print_int(re.h2_count);
    print_str(" PV_total="); print_int((int)(re.total_pv * 1000));
    print_str("W Wind="); print_int((int)(re.total_wind * 1000)); print_str("W\n");
}

int main(void) {
    print_str("=== Renewable Energy Modeling Demo ===\n\n");
    re_init();

    print_str("Adding PV arrays...\n");
    re_add_pv(PV_MONO, 100.0, 0.20, 30.0, 180.0);
    re_add_pv(PV_POLY, 80.0, 0.17, 25.0, 180.0);
    re_add_pv(PV_THINF, 150.0, 0.12, 20.0, 180.0);

    print_str("\nAdding wind turbines...\n");
    re_add_wind(WIND_HAWT, 80.0, 80.0, 2000000.0);
    re_add_wind(WIND_HAWT, 100.0, 100.0, 3000000.0);
    re_add_wind(WIND_OFFSHORE, 120.0, 90.0, 5000000.0);

    print_str("\nAdding storage systems...\n");
    re_add_storage(BATT_LION, 500.0, 50.0);
    re_add_storage(BATT_FLOW, 1000.0, 60.0);
    re_add_storage(BATT_PUMPED, 5000.0, 70.0);

    print_str("\nPV model with MPPT...\n");
    for (int i = 0; i < re.pv_count; i++) {
        re_pv_model(i, MPPT_PO);
    }

    print_str("\nWind turbine power...\n");
    for (int i = 0; i < re.wind_count; i++) {
        winds[i].wind_speed = weather.speed[12];
        re_wind_model(i);
    }

    print_str("\nWeibull wind distribution...\n");
    re_weibull_distribution(8.0, 2.0);

    print_str("\nStorage dispatch...\n");
    re_storage_dispatch(0, 100.0);
    re_storage_dispatch(1, -80.0);
    re_storage_dispatch(2, 200.0);

    print_str("\nForecast accuracy...\n");
    re_forecast_accuracy(24);

    print_str("\nLCOE calculation...\n");
    re.capacity_factor = 0.25;
    re_compute_lcoe(10000000.0, 200000.0, 25.0, 0.05);

    re.total_load = (re.total_pv + re.total_wind) * 1.5;
    re.renewable_fraction = (re.total_pv + re.total_wind) / (re.total_load + 1e-10);
    print_str("  Renewable fraction="); print_int((int)(re.renewable_fraction * 100));
    print_str("%\n");

    re_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
