#include "ViewFilter.hpp"

#include <Laplace/Fft.hpp>
#include <Laplace/Filter.hpp>

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <implot.h>

#include <algorithm>
#include <array>
#include <numeric>
#include <random>

using namespace std::chrono_literals;

namespace {

auto& rng()
{
    thread_local auto generator = []() {
        auto seed_data = std::array<std::random_device::result_type, std::mt19937::state_size>();
        auto random_device = std::random_device();
        std::generate_n(seed_data.data(), seed_data.size(), std::ref(random_device));
        auto sequence = std::seed_seq(seed_data.begin(), seed_data.end());
        return std::mt19937(sequence);
    }();
    return generator;
}

auto generate_signal()
{
    constexpr auto dc_gain = 0.5;
    constexpr auto duration = 15s;
    constexpr auto dt = 10ms;
    auto signal = lp::Signal<double>(duration / dt);
    auto noise = std::normal_distribution(0., .1);
    for (std::size_t i = 0; i < signal.size(); ++i) {
        const auto t = i * dt;
        const auto frequency = lp::radians_per_second((lp::hertz(0.2) * t).as_radians());
        signal[i] = { t, cos(frequency * t) + dc_gain + noise(rng()) };
    }
    return signal;
}

auto run_filter(lp::Filter<double>& filter, const lp::Signal<double>& input)
{
    assert(input.size() >= 2);
    const auto dt = std::chrono::duration_cast<std::chrono::nanoseconds>(input[1].time - input[0].time);
    auto output = lp::Signal<double>(input.size());
    for (std::size_t i = 0; i < output.size(); ++i) {
        output[i] = { input[i].time, filter.value() };
        filter(input[i].amplitude, dt);
    }
    return output;
}

auto signal_reader(int index, void* data)
{
    const auto [time, amplitude] = static_cast<lp::Signal<double>::value_type*>(data)[index];
    return ImPlotPoint(std::chrono::duration_cast<std::chrono::duration<double>>(time).count(), amplitude);
}

auto dft_reader(int index, void* data)
{
    const auto [frequency, phase, amplitude] = static_cast<std::vector<lp::DftDatum<double>>::value_type*>(data)[index];
    return ImPlotPoint(frequency.as_hertz(), amplitude);
}
}

void view_filter(const std::string& name, lp::Filter<double>& filter)
{
    auto window = sf::RenderWindow(sf::VideoMode({ 640, 480 }), name + " Example");
    window.setFramerateLimit(144);
    if (!ImGui::SFML::Init(window))
        throw std::runtime_error("Failed to initilalize ImGui");
    ImPlot::CreateContext();

    auto input = generate_signal();
    auto output = run_filter(filter, input);

    auto input_freqs = lp::fft(input);
    auto output_freqs = lp::fft(output);

    auto clock = sf::Clock();
    while (window.isOpen()) {
        for (auto event = sf::Event(); window.pollEvent(event);) {
            ImGui::SFML::ProcessEvent(window, event);
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.scancode == sf::Keyboard::Scan::Escape)
                    window.close();
                break;
            default:
                break;
            }
        }

        ImGui::SFML::Update(window, clock.restart());

        // Make GUI window
        const auto [width, height] = sf::Vector2f(window.getSize());
        ImGui::SetNextWindowSize({ width, height });
        ImGui::SetNextWindowPos({});
        ImGui::Begin(
            "Filter", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        const auto plot_size = ImVec2(width - 20, height / 2 - 20);

        // Plot time response
        if (!ImPlot::BeginPlot("##Time", plot_size, ImPlotFlags_NoTitle | ImPlotFlags_NoFrame))
            throw std::runtime_error("Failed to begin plot");
        ImPlot::SetupAxes("Time (s)", "Amplitude");
        ImPlot::PlotLineG("Input", signal_reader, input.data(), int(input.size()));
        ImPlot::PlotLineG("Output", signal_reader, output.data(), int(output.size()));
        ImPlot::EndPlot();

        // Plot frequency response
        if (!ImPlot::BeginPlot("##Frequency", plot_size, ImPlotFlags_NoTitle | ImPlotFlags_NoFrame))
            throw std::runtime_error("Failed to begin plot");
        ImPlot::SetupAxes("Frequency (Hz)", "Amplitude");
        ImPlot::PlotBarsG("Input", dft_reader, input_freqs.data(), int(input_freqs.size()), .15);
        ImPlot::PlotBarsG("Output", dft_reader, output_freqs.data(), int(output_freqs.size()), .1);
        ImPlot::EndPlot();

        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImPlot::DestroyContext();
    ImGui::SFML::Shutdown();
}
