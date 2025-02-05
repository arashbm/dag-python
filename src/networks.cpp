#include <vector>

#include "bind_core.hpp"
#include <nanobind/operators.h>
#include <fmt/format.h>

#include <reticula/networks.hpp>

#include "type_str/networks.hpp"
#include "type_utils.hpp"
#include "type_handles.hpp"

namespace nb = nanobind;
using namespace nanobind::literals;

template <reticula::network_edge EdgeT>
struct declare_network_class {
  void operator()(nb::module_ &m) {
    using Net = reticula::network<EdgeT>;
    nb::class_<Net>(m, python_type_str<Net>().c_str())
      .def(nb::init<>(),
          nb::call_guard<nb::gil_scoped_release>())
      .def(nb::init<reticula::network<EdgeT>>(),
          "network"_a,
          nb::call_guard<nb::gil_scoped_release>())
      .def(nb::init<std::vector<EdgeT>>(),
          "edges"_a,
          nb::call_guard<nb::gil_scoped_release>())
      .def(nb::init<
            std::vector<EdgeT>,
            std::vector<typename EdgeT::VertexType>>(),
          "edges"_a, "verts"_a,
          nb::call_guard<nb::gil_scoped_release>())
      .def("vertices",
          &Net::vertices,
          nb::call_guard<nb::gil_scoped_release>())
      .def("edges",
          &Net::edges,
          nb::call_guard<nb::gil_scoped_release>())
      .def("edges_cause",
          &Net::edges_cause,
          nb::call_guard<nb::gil_scoped_release>())
      .def("edges_effect",
          &Net::edges_effect,
          nb::call_guard<nb::gil_scoped_release>())
      .def("incident_edges",
          &Net::incident_edges,
          "vert"_a,
          nb::call_guard<nb::gil_scoped_release>())
      .def("in_degree",
          &Net::in_degree,
          "vert"_a,
          nb::call_guard<nb::gil_scoped_release>())
      .def("out_degree",
          &Net::out_degree,
          "vert"_a,
          nb::call_guard<nb::gil_scoped_release>())
      .def("degree",
          &Net::degree,
          "vert"_a,
          nb::call_guard<nb::gil_scoped_release>())
      .def("predecessors",
          &Net::predecessors,
          "vert"_a,
          nb::call_guard<nb::gil_scoped_release>())
      .def("successors",
          &Net::successors,
          "vert"_a,
          nb::call_guard<nb::gil_scoped_release>())
      .def("neighbours",
          &Net::neighbours,
          "vert"_a,
          nb::call_guard<nb::gil_scoped_release>())
      .def("in_edges",
          nb::overload_cast<const typename EdgeT::VertexType&>(
            &Net::in_edges, nb::const_),
          "vert"_a,
          nb::call_guard<nb::gil_scoped_release>())
      .def("out_edges",
          nb::overload_cast<const typename EdgeT::VertexType&>(
            &Net::out_edges, nb::const_),
          "vert"_a,
          nb::call_guard<nb::gil_scoped_release>())
      .def("in_edges",
          nb::overload_cast<>(
            &Net::in_edges, nb::const_),
          nb::call_guard<nb::gil_scoped_release>())
      .def("out_edges",
          nb::overload_cast<>(
            &Net::out_edges, nb::const_),
          nb::call_guard<nb::gil_scoped_release>())
      .def(nb::self == nb::self,
          nb::call_guard<nb::gil_scoped_release>())
      .def(nb::self != nb::self,
          nb::call_guard<nb::gil_scoped_release>())
      .def("__copy__", [](const Net& self) {
          return Net(self);
      }).def("__deepcopy__", [](const Net& self, nb::dict) {
          return Net(self);
      }, "memo"_a)
      .def("__repr__", [](const Net& a) {
        return fmt::format("{}", a);
      }).def_static("__class_repr__", []() {
        return fmt::format("<class '{}'>", type_str<Net>{}());
      }).def_static("__class_name__", []() {
        return type_str<Net>{}();
      }).def_static("edge_type", []() {
        return types::handle_for<typename Net::EdgeType>();
      }).def_static("vertex_type", []() {
        return types::handle_for<typename Net::VertexType>();
      });
  }
};

void declare_typed_networks(nb::module_& m) {
  types::run_each<
    metal::transform<
      metal::lambda<declare_network_class>,
      types::all_edge_types>>{}(m);
}
