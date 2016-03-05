/*
 * $HEADER$
 */
#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

extern const mca_base_component_t mca_pls_proxy_component;
extern const mca_base_component_t mca_pls_gridengine_component;
extern const mca_base_component_t mca_pls_rsh_component;
extern const mca_base_component_t mca_pls_slurm_component;

const mca_base_component_t *mca_pls_base_static_components[] = {
  &mca_pls_proxy_component, 
  &mca_pls_gridengine_component, 
  &mca_pls_rsh_component, 
  &mca_pls_slurm_component, 
  NULL
};

#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

