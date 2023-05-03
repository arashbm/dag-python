#include "bind_core.hpp"
#include <fmt/format.h>

#include <reticula/temporal_adjacency.hpp>

#include "type_str/temporal_adjacency.hpp"
#include "type_utils.hpp"
#include "type_handles.hpp"

namespace nb = nanobind;
using namespace nanobind::literals;

template <reticula::temporal_network_edge EdgeT>
struct declare_temporal_adjacency_class {
  void operator()(nb::module_& m) {
    using Simple = reticula::temporal_adjacency::simple<EdgeT>;
    nb::class_<Simple>(
        m, python_type_str<Simple>().c_str())
      .def(nb::init<>())
      .def("linger",
          &Simple::linger,
          nb::call_guard<nb::gil_scoped_release>())
      .def("maximum_linger",
          &Simple::maximum_linger,
          nb::call_guard<nb::gil_scoped_release>())
      .def_static("edge_type", []() {
        return types::handle_for<typename Simple::EdgeType>();
      }).def_static("vertex_type", []() {
        return types::handle_for<typename Simple::VertexType>();
      }).def_static("__class_repr__", []() {
        return fmt::format("<class '{}'>", type_str<Simple>{}());
      });

    using LWT = reticula::temporal_adjacency::limited_waiting_time<EdgeT>;
    nb::class_<LWT>(
        m, python_type_str<LWT>().c_str())
      .def(nb::init<typename EdgeT::TimeType>(),
          "dt"_a,
          nb::call_guard<nb::gil_scoped_release>())
      .def("linger",
          &LWT::linger,
          nb::call_guard<nb::gil_scoped_release>())
      .def("maximum_linger",
          &LWT::maximum_linger,
          nb::call_guard<nb::gil_scoped_release>())
      .def("dt",
          &LWT::dt,
          nb::call_guard<nb::gil_scoped_release>())
      .def_static("edge_type", []() {
        return types::handle_for<typename LWT::EdgeType>();
      }).def_static("vertex_type", []() {
        return types::handle_for<typename LWT::VertexType>();
      }).def_static("__class_repr__", []() {
        return fmt::format("<class '{}'>", type_str<LWT>{}());
      });

    if constexpr (std::is_floating_point_v<typename EdgeT::TimeType>) {
      using Exp = reticula::temporal_adjacency::exponential<EdgeT>;
      nb::class_<Exp>(
          m, python_type_str<Exp>().c_str())
        .def(nb::init<typename EdgeT::TimeType, std::size_t>(),
            "rate"_a, "seed"_a,
            nb::call_guard<nb::gil_scoped_release>())
        .def("linger",
            &Exp::linger,
            nb::call_guard<nb::gil_scoped_release>())
        .def("maximum_linger",
            &Exp::maximum_linger,
            nb::call_guard<nb::gil_scoped_release>())
        .def("rate",
            &Exp::rate,
            nb::call_guard<nb::gil_scoped_release>())
        .def_static("edge_type", []() {
          return types::handle_for<typename Exp::EdgeType>();
        }).def_static("vertex_type", []() {
          return types::handle_for<typename Exp::VertexType>();
        }).def_static("__class_repr__", []() {
          return fmt::format("<class '{}'>", type_str<Exp>{}());
        });
    }

    if constexpr (std::is_integral_v<typename EdgeT::TimeType>) {
      using Geom = reticula::temporal_adjacency::geometric<EdgeT>;
      nb::class_<Geom>(
          m, python_type_str<Geom>().c_str())
        .def(nb::init<double, std::size_t>(),
            "p"_a, "seed"_a,
            nb::call_guard<nb::gil_scoped_release>())
        .def("linger",
            &Geom::linger,
            nb::call_guard<nb::gil_scoped_release>())
        .def("maximum_linger",
            &Geom::maximum_linger,
            nb::call_guard<nb::gil_scoped_release>())
        .def("p",
            &Geom::p,
            nb::call_guard<nb::gil_scoped_release>())
        .def_static("edge_type", []() {
          return types::handle_for<typename Geom::EdgeType>();
        }).def_static("vertex_type", []() {
          return types::handle_for<typename Geom::VertexType>();
        }).def_static("__class_repr__", []() {
          return fmt::format("<class '{}'>", type_str<Geom>{}());
        });
    }
  }
};

void declare_typed_temporal_adjacency_class(nb::module_& m) {
  // declare network
  types::run_each<
    metal::transform<
      metal::lambda<declare_temporal_adjacency_class>,
      types::first_order_temporal_edges>>{}(m);
}
