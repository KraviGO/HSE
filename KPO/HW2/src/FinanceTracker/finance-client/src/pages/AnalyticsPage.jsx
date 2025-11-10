import React from "react";
import { useQuery } from "@tanstack/react-query";
import { api } from "../api/api";
import { Alert, Typography, Box, Paper } from "@mui/material";
import { useCurrentUser } from "../state/UserContext";
import ExpensesChart from "../components/Charts/ExpensesChart";
import IncomeChart from "../components/Charts/IncomeChart";
import BalanceOverTime from "../components/Charts/BalanceOverTime";
import styles from "./AnalyticsPage.module.css";

export default function AnalyticsPage() {
    const { user } = useCurrentUser();
    const enabled = !!user?.id;

    const qOps = useQuery({
        queryKey: ["operations", user?.id],
        queryFn: async () => (await api.get(`/operations/${user.id}`)).data,
        enabled,
    });

    if (!user) return <Alert severity="info">Выберите пользователя.</Alert>;

    const ops = qOps.data ?? [];

    const expensesByCat = Object.values(
        ops
            .filter((o) => o.amount < 0)
            .reduce((acc, o) => {
                const name = o.categoryName ?? "Без категории";
                acc[name] = acc[name] || { name, value: 0 };
                acc[name].value += Math.abs(o.amount);
                return acc;
            }, {})
    );

    const incomeByCat = Object.values(
        ops
            .filter((o) => o.amount > 0)
            .reduce((acc, o) => {
                const name = o.categoryName ?? "Без категории";
                acc[name] = acc[name] || { name, value: 0 };
                acc[name].value += o.amount;
                return acc;
            }, {})
    );

    const byDate = Object.values(
        ops.reduce((acc, o) => {
            const d = o.occurredOn;
            acc[d] = acc[d] || { date: d, value: 0 };
            acc[d].value += o.amount;
            return acc;
        }, {})
    ).sort((a, b) => a.date.localeCompare(b.date));

    return (
        <Box className={styles.container}>
            <Typography variant="h5" className={styles.title}>
                📊 Аналитика
            </Typography>

            <Box className={styles.row}>
                <Paper className={styles.card}>
                    <Typography variant="h6" className={styles.cardTitle}>
                        💸 Расходы по категориям
                    </Typography>
                    <div className={styles.chart}>
                        <ExpensesChart data={expensesByCat} />
                    </div>
                </Paper>

                <Paper className={styles.card}>
                    <Typography variant="h6" className={styles.cardTitle}>
                        💰 Доходы по категориям
                    </Typography>
                    <div className={styles.chart}>
                        <IncomeChart data={incomeByCat} />
                    </div>
                </Paper>
            </Box>

            <Box className={styles.row}>
                <Paper className={`${styles.card} ${styles.full}`}>
                    <Typography variant="h6" className={styles.cardTitle}>
                        📈 Динамика по датам
                    </Typography>
                    <div className={styles.chart}>
                        <BalanceOverTime data={byDate} />
                    </div>
                </Paper>
            </Box>
        </Box>
    );
}