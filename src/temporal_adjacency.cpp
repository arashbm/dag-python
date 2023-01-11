#include <pybind11/pybind11.h>
#include <fmt/format.h>

#include <reticula/temporal_adjacency.hpp>

#include "type_str/temporal_adjacency.hpp"
#include "type_utils.hpp"
#include "type_handles.hpp"
#include "metaclass.hpp"

namespace py = pybind11;
using namespace pybind11::literals;

template <reticula::temporal_network_edge EdgeT>
struct declare_temporal_adjacency_class {
  void operator()(py::module &m, PyObject* metaclass) {
    using Simple = reticula::temporal_adjacency::simple<EdgeT>;
    py::class_<Simple>(
        m, python_type_str<Simple>().c_str(), py::metaclass(metaclass))
      .def(py::init<>())
      .def("linger",
          &Simple::linger,
          py::call_guard<py::gil_scoped_release>())
      .def("maximum_linger",
          &Simple::maximum_linger,
          py::call_guard<py::gil_scoped_release>())
      .def_static("edge_type", []() {
        return types::handle_for<typename Simple::EdgeType>();
      }).def_static("vertex_type", []() {
        return types::handle_for<typename Simple::VertexType>();
      }).def_static("__class_repr__", []() {
        return fmt::format("<class '{}'>", type_str<Simple>{}());
      });

    using LWT = reticula::temporal_adjacency::limited_waiting_time<EdgeT>;
    py::class_<LWT>(
        m, python_type_str<LWT>().c_str(), py::metaclass(metaclass))
      .def(py::init<typename EdgeT::TimeType>(),
          "dt"_a,
          py::call_guard<py::gil_scoped_release>())
      .def("linger",
          &LWT::linger,
          py::call_guard<py::gil_scoped_release>())
      .def("maximum_linger",
          &LWT::maximum_linger,
          py::call_guard<py::gil_scoped_release>())
      .def("dt",
          &LWT::dt,
          py::call_guard<py::gil_scoped_release>())
      .def_static("edge_type", []() {
        return types::handle_for<typename LWT::EdgeType>();
      }).def_static("vertex_type", []() {
        return types::handle_for<typename LWT::VertexType>();
      }).def_static("__class_repr__", []() {
        return fmt::format("<class '{}'>", type_str<LWT>{}());
      });

    if constexpr (std::is_floating_point_v<typename EdgeT::TimeType>) {
      using Exp = reticula::temporal_adjacency::exponential<EdgeT>;
      py::class_<Exp>(
          m, python_type_str<Exp>().c_str(), py::metaclass(metaclass))
        .def(py::init<typename EdgeT::TimeType, std::size_t>(),
            "rate"_a, "seed"_a,
            py::call_guard<py::gil_scoped_release>())
        .def("linger",
            &Exp::linger,
            py::call_guard<py::gil_scoped_release>())
        .def("maximum_linger",
            &Exp::maximum_linger,
            py::call_guard<py::gil_scoped_release>())
        .def("rate",
            &Exp::rate,
            py::call_guard<py::gil_scoped_release>())
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
      py::class_<Geom>(
          m, python_type_str<Geom>().c_str(), py::metaclass(metaclass))
        .def(py::init<double, std::size_t>(),
            "p"_a, "seed"_a,
            py::call_guard<py::gil_scoped_release>())
        .def("linger",
            &Geom::linger,
            py::call_guard<py::gil_scoped_release>())
        .def("maximum_linger",
            &Geom::maximum_linger,
            py::call_guard<py::gil_scoped_release>())
        .def("p",
            &Geom::p,
            py::call_guard<py::gil_scoped_release>())
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

void declare_typed_temporal_adjacency_class(py::module& m) {
  auto metaclass = common_metaclass("temporal_adjacency_metaclass");

  // declare network
  types::run_each<
    metal::transform<
      metal::lambda<declare_temporal_adjacency_class>,
      types::first_order_temporal_edges>>{}(m, (PyObject*)metaclass);
}
