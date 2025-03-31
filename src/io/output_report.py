import plotly.graph_objects as go
import re

class OutputReport:
    results: dict
    
    def __init__(self, results):
        self.results = results

    def plot_convergence(self):
        fig = go.Figure()

        for instance, instance_results in self.results.items():
            # Get the result with the longest CONVERGENCE_REPORT
            best_result = max(instance_results, key=lambda c: len(c["CONVERGENCE_REPORT"]))
            convergence_report = best_result["CONVERGENCE_REPORT"]
            
            # Extract generations and best_fitness values
            generations = [entry["generation"] for entry in convergence_report]
            best_fitness = [entry["best_fitness"] for entry in convergence_report]

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
            template="plotly_dark",
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
                "BRKGA": int(min(r["RESULT"]["Min_f(x)"] for r in instance_results)),
                "BRKGA Time": sum(r["EXECUTION_TIME_REPORT"]["duration_seconds"] for r in instance_results) / len(instance_results),
                "SOA": instance_results[0]["INSTANCE"]["SOA_Cost"]
            }
            for instance, instance_results in self.results.items()
        }
        # Function to extract the numerical part from the instance name
        def extract_number(instance_name):
            match = re.search(r'(\d+)', instance_name)  # Look for the first sequence of digits
            return int(match.group(1)) if match else float('inf')  # Return the number or inf if no match

        # Sort instance names based on the numerical part
        instance_names = sorted(comparison.keys(), key=extract_number)
        brkga_values = [comparison[instance]["BRKGA"] for instance in instance_names]
        soa_values = [comparison[instance]["SOA"] for instance in instance_names]
        
        
        # Assign colors and labels
        brkga_colors = []
        soa_colors = []
        brkga_texts = []
        soa_texts = []

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
            template='plotly_dark',  # For a sleek look
            coloraxis_showscale=False,  # No color scale needed
            xaxis_tickangle=-45  # Rotate x-axis labels for better visibility
        )

        # Show plot
        fig.show()