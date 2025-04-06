import plotly.graph_objects as go
import numpy as np
import re

class OutputReport:
    results: dict
    
    def __init__(self, results):
        self.results = results
        self.template = 'plotly_dark'
        self.soa_execution_time = {
            "o-5_t": {"HS_RS": 0.72, "WSA": 7.71},
            "o-5_t_2": {"HS_RS": 1.55, "WSA": 7.63},
            "o-6_t": {"HS_RS": 1.40, "WSA": 8.07},
            "o-7_t": {"HS_RS": 1.57, "WSA": 8.6},
            "o-8_t": {"HS_RS": 1.84, "WSA": 9.56},
            "o-9_t": {"HS_RS": 1.90, "WSA": 10},
            "o-10_t": {"HS_RS": 1.22, "WSA": 10.43},
            "o-10_t_2": {"HS_RS": 1.50, "WSA": 10.44},
            "o-10_t_3": {"HS_RS": 1.69, "WSA": 10.42},
            "o-10_t_4": {"HS_RS": 3.00, "WSA": 10.37},
            "o-10_t_5": {"HS_RS": 2.01, "WSA": 10.34},
            "o-10_t_6": {"HS_RS": 2.33, "WSA": 10.32},
            "o-15_t": {"HS_RS": 3.32, "WSA": 12.61},
            "o-15_t_2": {"HS_RS": 2.59, "WSA": 12.57},
            "O-20_t": {"HS_RS": 4.31, "WSA": 15.24},
            "s-12_t": {"HS_RS": 3.18, "WSA": 12.15},
            "s-13_t": {"HS_RS": 3.12, "WSA": 13.38},
            "s-13_t_2": {"HS_RS": 2.47, "WSA": 13.34},
            "y-6_t": {"HS_RS": 0.70, "WSA": 8.26},
            "y-6_t_2": {"HS_RS": 1.51, "WSA": 8.14},
            "y-7_t": {"HS_RS": 1.91, "WSA": 8.5},
            "y-8_t": {"HS_RS": 0.46, "WSA": 9.54},
            "y-9_t": {"HS_RS": 0.46, "WSA": 9.96},
            "y-10_t": {"HS_RS": 1.23, "WSA": 10.36},
            "y-11_t": {"HS_RS": 1.47, "WSA": 10.8},
            "y-11_t_2": {"HS_RS": 1.58, "WSA": 10.77},
            "y-12_t": {"HS_RS": 1.63, "WSA": 11.12},
            "y-13_t": {"HS_RS": 1.85, "WSA": 11.66},
            "y-14_t": {"HS_RS": 1.84, "WSA": 12.12},
            "y-15_t": {"HS_RS": 2.62, "WSA": 12.67},
            "y-20_t": {"HS_RS": 4.26, "WSA": 15.13},
            "sko-36": {"HS_RS": 5.01, "WSA": 26.29},
            "sko-42": {"HS_RS": 5.41, "WSA": 32.1},
            "sko-49": {"HS_RS": 5.58, "WSA": 40.68},
            "sko-56": {"HS_RS": 5.75, "WSA": 49.8},
            "sko-64": {"HS_RS": 8.80, "WSA": 118.34},
            "sko-72": {"HS_RS": 9.58, "WSA": 78.13},
            "sko-81": {"HS_RS": 10.19, "WSA": 171.81},
            "sko-100": {"HS_RS": 12.71, "WSA": 271.23},
            "y-30_t": {"HS_RS": 5.30, "WSA": 21.62},
            "y-35_t": {"HS_RS": 5.59, "WSA": 25.53},
            "y-40_t": {"HS_RS": 5.40, "WSA": 29.87},
            "y-45_t": {"HS_RS": 6.32, "WSA": 35.18},
            "y-50_t": {"HS_RS": 6.15, "WSA": 43.94},
            "y-60_t": {"HS_RS": 7.74, "WSA": 55.85},
            "anjos-60-01": {"HS_RS": 6.85, "WSA": 56.62},
            "anjos-60-02": {"HS_RS": 6.80, "WSA": 57.25},
            "anjos-60-03": {"HS_RS": 6.99, "WSA": 59.02},
            "anjos-60-04": {"HS_RS": 7.45, "WSA": 62.17},
            "anjos-60-05": {"HS_RS": 7.36, "WSA": 65.3},
            "anjos-70-01": {"HS_RS": 7.43, "WSA": 88.17},
            "anjos-70-02": {"HS_RS": 7.49, "WSA": 85.99},
            "anjos-70-03": {"HS_RS": 7.89, "WSA": 89.69},
            "anjos-70-04": {"HS_RS": 8.37, "WSA": 86.81},
            "anjos-70-05": {"HS_RS": 7.29, "WSA": 85.58},
            "anjos-75-01": {"HS_RS": 7.96, "WSA": 101.47},
            "anjos-75-02": {"HS_RS": 7.60, "WSA": 95.75},
            "anjos-75-03": {"HS_RS": 8.19, "WSA": 96.1},
            "anjos-75-04": {"HS_RS": 8.57, "WSA": 97.48},
            "anjos-75-05": {"HS_RS": 8.05, "WSA": 97.89},
            "anjos-80-01": {"HS_RS": 8.67, "WSA": 109.42},
            "anjos-80-02": {"HS_RS": 8.44, "WSA": 108.35},
            "anjos-80-03": {"HS_RS": 8.44, "WSA": 106.72},
            "anjos-80-04": {"HS_RS": 9.15, "WSA": 103.53},
            "anjos-80-05": {"HS_RS": 8.78, "WSA": 101.13}
        }

    def plot_convergence(self):
        fig = go.Figure()

        for instance, instance_results in self.results.items():
            # Get the result with the longest CONVERGENCE_REPORT
            best_result = min(instance_results, key=lambda c: c["result"]["minFx"])
            convergence_report = best_result["convergenceReport"]
            
            # Extract generations and best_fitness values
            generations = [entry["generation"] for entry in convergence_report]
            best_fitness = [entry["bestFitness"] for entry in convergence_report]

            # Add a trace for each instance
            fig.add_trace(go.Scatter(
                x=generations, 
                y=best_fitness, 
                mode='lines+markers', 
                name=f'Instance {instance}',
                line=dict(width=2),
                marker=dict(size=6),
                hoverinfo="x+y+name"
            ))

        # Improve layout aesthetics
        fig.update_layout(
            title="Convergence Report for All Instances",
            xaxis_title="Generation",
            yaxis_title="Best Fitness",
            template=self.template,
            hovermode="x unified",
            font=dict(size=14),
            margin=dict(l=50, r=50, t=50, b=50),
            width=900, height=600,
            legend=dict(title="Instances", bgcolor="rgba(0,0,0,0.5)")
        )

        fig.show()
    
    def plot_comparison(self):
        comparison = {
            instance: {
                "BRKGA": int(min(r["result"]["minFx"] for r in instance_results)),
                "BRKGA Time": sum(r["executionTimeReport"]["durationSeconds"] for r in instance_results) / len(instance_results),
                "SOA": instance_results[0]["instance"]["soaCost"]
            }
            for instance, instance_results in self.results.items()
        }

        # Sort instance names based on the numerical part
        instance_names = sorted(comparison.keys(), key=self.extract_number)
        brkga_values = [comparison[instance]["BRKGA"] for instance in instance_names]
        soa_values = [comparison[instance]["SOA"] for instance in instance_names]
        
        # Assign colors and labels
        brkga_colors = []
        soa_colors = []

        for brkga, soa in zip(brkga_values, soa_values):
            if brkga == soa:
                brkga_colors.append('#FFD700')  # Yellow
                soa_colors.append('#FFD700')    # Yellow
            elif brkga > soa:
                brkga_colors.append('#0000FF')  # Blue
                soa_colors.append('#ff0000')    # Red
            else:
                brkga_colors.append('#0000FF')  # Blue
                soa_colors.append('#ff0000')    # Red
        
        # Create the grouped bar plot
        fig = go.Figure(data=[
            go.Bar(
                x=instance_names, 
                y=brkga_values,
                name="BRKGA",
                text=brkga_values,  # Display values on bars
                textposition='outside',  # Position text on top of bars
                hovertext=[f"BRKGA: {comparison[instance]['BRKGA']}" for instance in instance_names],
                hoverinfo='text',
                marker=dict(color=brkga_colors)
            ),
            go.Bar(
                x=instance_names,
                y=soa_values,
                name="SOA",
                text=soa_values,  # Display values on bars
                textposition='outside',  # Position text on top of bars
                hovertext=[f"SOA: {soa}" for soa in soa_values],
                hoverinfo='text',
                marker=dict(color=soa_colors)
            )
        ])

        # Update layout for better visualization
        fig.update_layout(
            title='Comparison of BRKGA and SOA Costs for Each Instance',
            xaxis_title='Instance Name',
            yaxis_title='Cost',
            barmode='group',  # Group the bars side by side
            template=self.template,  # For a sleek look
            coloraxis_showscale=False,  # No color scale needed
            xaxis_tickangle=-45  # Rotate x-axis labels for better visibility
        )

        # Show plot
        fig.show()

    def plot_lsreport(self):
        lsr_sum = {'swap': 0, 'reinsertion': 0, 'twoOpt': 0}

        for instance_results in self.results.values():
            for result in instance_results:
                report = result.get('localSearchReport', {})
                for key in lsr_sum.keys():
                    lsr_sum[key] += report.get(key, 0)

        # Create bar chart
        fig = go.Figure(data=[
            go.Bar(
                x=[k.capitalize() for k in lsr_sum.keys()],
                y=list(lsr_sum.values()),  # Cast to list here
                text=list(lsr_sum.values()),
                textposition='outside',
                marker_color=['#FF0000', '#0000FF', '#00FF00'],
                hoverinfo='x+y'
            )
        ])

        # Update layout
        fig.update_layout(
            title="Aggregated Local Search Report",
            yaxis_title="Improvements",
            xaxis_title="Local Search",
            template=self.template,
            width=1000,
            height=500,
            font=dict(size=12),
            margin=dict(l=40, r=40, t=50, b=50)
        )

        fig.show()

    def extract_number(self, instance_name):
        match = re.search(r'(\d+)', instance_name)
        return int(match.group(1)) if match else float('inf')

    def plot_execution_time_comparison(self, compare_to: str):
        # Extract BRKGA execution times (average durationSeconds per instance)
        brkga = {
            instance[:-4]: np.mean([ir["executionTimeReport"]["durationSeconds"] for ir in instance_results])
            for instance, instance_results in self.results.items()
        }

        # Extract SoA execution times for the given method ("HS_RS" or "WSA")
        soa = {
            instance: r[compare_to]
            for instance, r in self.soa_execution_time.items()
        }

        # Match instances available in both sources
        common_instances = sorted(set(brkga.keys()) & set(soa.keys()))

        # Sort instances by numeric value
        sorted_instances = sorted(common_instances, key=self.extract_number)

        brkga_times = [brkga[inst] for inst in sorted_instances]
        soa_times = [soa[inst] for inst in sorted_instances]

        # Create the horizontal bar chart
        fig = go.Figure(data=[
            go.Bar(
                y=sorted_instances,
                x=soa_times,
                name=f'SOA ({compare_to})',
                orientation='h',
                marker_color='#FF0000',
                text=[f"{x:.2f}s" for x in soa_times],
                textposition='outside',
                hoverinfo='x+y+name'
            ),
            go.Bar(
                y=sorted_instances,
                x=brkga_times,
                name='BRKGA',
                orientation='h',
                marker_color='#0000FF',
                text=[f"{x:.2f}s" for x in brkga_times],
                textposition='outside',
                hoverinfo='x+y+name'
            )
        ])

        fig.update_layout(
            title=f'Execution Time Comparison (BRKGA vs {compare_to})',
            xaxis_title='Execution Time (seconds)',
            yaxis_title='Instance',
            barmode='group',
            template=self.template,
            width=1000,
            height=600 + len(sorted_instances) * 10,
            font=dict(size=12),
            margin=dict(l=100, r=50, t=50, b=50)
        )

        fig.show()
