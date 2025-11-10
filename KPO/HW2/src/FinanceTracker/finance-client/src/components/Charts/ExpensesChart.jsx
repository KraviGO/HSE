import React from "react";
import { PieChart, Pie, Cell, Tooltip, Legend, ResponsiveContainer } from "recharts";

const COLORS = ["#ef5350", "#66bb6a", "#42a5f5", "#ffa726", "#ab47bc", "#26a69a", "#ec407a"];

export default function ExpensesChart({ data }) {
    return (
        <ResponsiveContainer width="100%" height={320}>
            <PieChart>
                <Pie data={data} dataKey="value" nameKey="name" label outerRadius={120}>
                    {data.map((_, i) => <Cell key={i} fill={COLORS[i % COLORS.length]} />)}
                </Pie>
                <Tooltip />
                <Legend />
            </PieChart>
        </ResponsiveContainer>
    );
}