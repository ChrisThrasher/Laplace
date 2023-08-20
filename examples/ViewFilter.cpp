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

[[nodiscard]] auto& rng()
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

[[nodiscard]] auto generate_signal()
{
    constexpr auto dc_gain = 0.5;
    constexpr auto duration = 15s;
    constexpr auto dt = 10ms;

    auto signal = std::vector<double>(duration / dt);
    auto noise = std::normal_distribution(0., .1);
    for (std::size_t i = 0; i < signal.size(); ++i) {
        const auto t = i * dt;
        const auto frequency = lp::radians_per_second((lp::hertz(0.2) * t).as_radians());
        signal[i] = { cos(frequency * t) + dc_gain + noise(rng()) };
    }

    return std::make_pair(signal, dt);
}

[[nodiscard]] auto
run_filter(lp::Filter<double>& filter, const std::vector<double>& input, const std::chrono::nanoseconds dt)
{
    assert(input.size() >= 2);
    auto output = std::vector<double>(input.size());
    for (std::size_t i = 0; i < output.size(); ++i) {
        output[i] = filter.value();
        filter(input[i], dt);
    }
    return output;
}

[[nodiscard]] auto dft_reader(int index, void* data)
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

    const auto [input, dt] = generate_signal();
    const auto output = run_filter(filter, input, dt);
    const auto signal_width = std::chrono::duration_cast<std::chrono::duration<double>>(dt).count();

    auto input_freqs = lp::fft(input, dt);
    auto output_freqs = lp::fft(output, dt);
    const auto input_width = (input_freqs[1].frequency - input_freqs[0].frequency).as_hertz();
    const auto output_width = input_width * 0.6;

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
        ImPlot::PlotLine("Input", input.data(), int(input.size()), signal_width);
        ImPlot::PlotLine("Output", output.data(), int(output.size()), signal_width);
        ImPlot::EndPlot();

        // Plot frequency response
        if (!ImPlot::BeginPlot("##Frequency", plot_size, ImPlotFlags_NoTitle | ImPlotFlags_NoFrame))
            throw std::runtime_error("Failed to begin plot");
        ImPlot::SetupAxes("Frequency (Hz)", "Amplitude");
        ImPlot::PlotBarsG("Input", dft_reader, input_freqs.data(), int(input_freqs.size()), input_width);
        ImPlot::PlotBarsG("Output", dft_reader, output_freqs.data(), int(output_freqs.size()), output_width);
        ImPlot::EndPlot();

        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImPlot::DestroyContext();
    ImGui::SFML::Shutdown();
}
